'Based on: https://blogs.msdn.com/b/alejacma/archive/2008/12/11/how-to-sign-exe-files-with-an-authenticode-certificate-part-2.aspx
Imports System.ComponentModel
Imports System.Runtime.InteropServices
Imports System.Security.Cryptography
Imports System.Security.Cryptography.X509Certificates
Imports System.IO

Public Class Crypto

    ' #define CRYPTUI_WIZ_NO_UI     1
    Public Const CRYPTUI_WIZ_NO_UI As Int32 = 1

    ' #define CRYPTUI_WIZ_DIGITAL_SIGN_SUBJECT_FILE     0x01
    Public Const CRYPTUI_WIZ_DIGITAL_SIGN_SUBJECT_FILE As Int32 = 1

    ' #define CRYPTUI_WIZ_DIGITAL_SIGN_CERT                    0x01
    Public Const CRYPTUI_WIZ_DIGITAL_SIGN_CERT As Int32 = 1

    ' typedef struct _CRYPTUI_WIZ_DIGITAL_SIGN_INFO {  
    '   DWORD dwSize;  
    '   DWORD dwSubjectChoice;  
    '   union {    
    '       LPCWSTR pwszFileName;    
    '       PCCRYPTUI_WIZ_DIGITAL_SIGN_BLOB_INFO pSignBlobInfo;  
    '   };  
    '   DWORD dwSigningCertChoice;  
    '   union {    
    '       PCCERT_CONTEXT pSigningCertContext;    
    '       PCCRYPTUI_WIZ_DIGITAL_SIGN_STORE_INFO pSigningCertStore;    
    '       PCCRYPTUI_WIZ_DIGITAL_SIGN_CERT_PVK_INFO pSigningCertPvkInfo;  
    '   };  
    '   LPCWSTR pwszTimestampURL;  
    '   DWORD dwAdditionalCertChoice;  
    '   PCCRYPTUI_WIZ_DIGITAL_SIGN_EXTENDED_INFO pSignExtInfo;
    ' } CRYPTUI_WIZ_DIGITAL_SIGN_INFO;
    <StructLayout(LayoutKind.Sequential)> _
    Public Structure CRYPTUI_WIZ_DIGITAL_SIGN_INFO
        Public dwSize As Int32
        Public dwSubjectChoice As Int32
        <MarshalAs(UnmanagedType.LPWStr)> Public pwszFileName As String
        Public dwSigningCertChoice As Int32
        Public pSigningCertContext As IntPtr
        Public pwszTimestampURL As String
        Public dwAdditionalCertChoice As Int32
        Public pSignExtInfo As IntPtr
    End Structure

    ' typedef struct _CRYPTUI_WIZ_DIGITAL_SIGN_CONTEXT {  
    '      DWORD dwSize;  
    '      DWORD cbBlob;  
    '      BYTE* pbBlob;
    ' } CRYPTUI_WIZ_DIGITAL_SIGN_CONTEXT;
    <StructLayout(LayoutKind.Sequential)> _
    Public Structure CRYPTUI_WIZ_DIGITAL_SIGN_CONTEXT
        Public dwSize As Int32
        Public cbBlob As Int32
        Public pbBlob As IntPtr
    End Structure

    ' BOOL WINAPI CryptUIWizDigitalSign(
    '      DWORD dwFlags,
    '      HWND hwndParent,
    '      LPCWSTR pwszWizardTitle,
    '      PCCRYPTUI_WIZ_DIGITAL_SIGN_INFO pDigitalSignInfo,
    '      PCCRYPTUI_WIZ_DIGITAL_SIGN_CONTEXT* ppSignContext
    ' );
    <DllImport("Cryptui.dll", CharSet:=CharSet.Unicode, SetLastError:=True)> _
    Public Shared Function CryptUIWizDigitalSign( _
        ByVal dwFlags As Int32, _
        ByVal hwndParent As IntPtr, _
        ByVal pwszWizardTitle As String, _
        ByRef pDigitalSignInfo As CRYPTUI_WIZ_DIGITAL_SIGN_INFO, _
        ByRef ppSignContext As IntPtr _
    ) As Boolean
    End Function

    ' BOOL WINAPI CryptUIWizFreeDigitalSignContext(
    '   PCCRYPTUI_WIZ_DIGITAL_SIGN_CONTEXT pSignContext
    ' );
    <DllImport("Cryptui.dll", CharSet:=CharSet.Auto, SetLastError:=True)> _
    Public Shared Function CryptUIWizFreeDigitalSignContext( _
        ByVal pSignContext As IntPtr _
    ) As Boolean
    End Function

End Class

Module Module1

    Sub Main()

        ' Parameters
        Dim certPath As String = "MyCert.pfx"
        Dim exePath As String = "MyApplication.exe"
        Dim sigPath As String = "signature.sig"

        ' Variables
        '
        Dim cert As X509Certificate2
        Dim digitalSignInfo As Crypto.CRYPTUI_WIZ_DIGITAL_SIGN_INFO
        Dim pSignContext As IntPtr
        Dim pSigningCertContext As IntPtr
        Dim signContext As Crypto.CRYPTUI_WIZ_DIGITAL_SIGN_CONTEXT
        Dim fileOut As FileStream
        Dim binWriter As BinaryWriter
        Dim blob() As Byte

        Try
            ' Get certificate context
            '
            cert = New X509Certificate2(certPath, "")
            pSigningCertContext = cert.Handle

            ' Prepare signing info: exe and cert
            '
            digitalSignInfo = New Crypto.CRYPTUI_WIZ_DIGITAL_SIGN_INFO
            digitalSignInfo.dwSize = Marshal.SizeOf(digitalSignInfo)
            digitalSignInfo.dwSubjectChoice = Crypto.CRYPTUI_WIZ_DIGITAL_SIGN_SUBJECT_FILE
            digitalSignInfo.pwszFileName = exePath
            digitalSignInfo.dwSigningCertChoice = Crypto.CRYPTUI_WIZ_DIGITAL_SIGN_CERT
            digitalSignInfo.pSigningCertContext = pSigningCertContext
            digitalSignInfo.pwszTimestampURL = vbNullString
            digitalSignInfo.dwAdditionalCertChoice = 0
            digitalSignInfo.pSignExtInfo = IntPtr.Zero

            ' Sign exe
            '
            If (Not Crypto.CryptUIWizDigitalSign( _
                Crypto.CRYPTUI_WIZ_NO_UI, _
                IntPtr.Zero, _
                vbNullString, _
                digitalSignInfo, _
                pSignContext _
            )) Then
                Throw New Win32Exception(Marshal.GetLastWin32Error(), "CryptUIWizDigitalSign")
            End If

            ' Get the blob with the signature
            '
            signContext = Marshal.PtrToStructure(pSignContext, GetType(Crypto.CRYPTUI_WIZ_DIGITAL_SIGN_CONTEXT))
            blob = New Byte(signContext.cbBlob) {}
            Marshal.Copy(signContext.pbBlob, blob, 0, signContext.cbBlob)

            ' Store the signature in a new file
            '
            fileOut = File.Open(sigPath, FileMode.Create)
            binWriter = New BinaryWriter(fileOut)
            binWriter.Write(blob)
            binWriter.Close()
            fileOut.Close()

            ' Free blob
            '
            If (Not Crypto.CryptUIWizFreeDigitalSignContext(pSignContext)) Then
                Throw New Win32Exception(Marshal.GetLastWin32Error(), "CryptUIWizFreeDigitalSignContext")
            End If

            ' We are done
            Console.WriteLine("Done!!!")

        Catch ex As Win32Exception
            ' Any expected errors?
            '
            Console.WriteLine(ex.Message + " error#" + ex.NativeErrorCode.ToString)
        Catch ex As Exception
            ' Any unexpected errors?
            '
            Console.WriteLine(ex.Message)
        End Try

        ' We are done
        '
        Console.WriteLine("<< Press any key to continue >>")
        Console.ReadKey()

    End Sub

End Module