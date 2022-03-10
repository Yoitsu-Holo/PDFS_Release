#ifndef PROTOCOL_H
#define PROTOCOL_H

#endif // PROTOCOL_H

#define RunStateCode char
#define RSC RunStateCode

//server ans port
#define port_UI                 9999
#define port_Upload             9999
#define port_Download           9999

//word lenth
#define opPart_1                20
#define opPart_2                40
#define opPart_3                60
#define Length_Username         20
#define Length_Password         20
#define Length_Key              20

//客户端运行内部状态码
#define run_NoError             0
#define run_UnknowError         1
#define run_IllegalInput        2
#define run_FileExist           3
#define run_DirExist            4
#define run_FileUnexist         5
#define run_DirUnexist          6
#define run_IllegalIp           7
#define run_IllegalPort         8

//请求op代码
#define opCode_noError          0
#define opCode_CreateUser       1
#define opCode_DeleteUser       2
#define opCode_ChangePassword   3
#define opCode_Login            4
#define opCode_SendFile         5
#define opCode_DownloadFile     6
#define opCode_DeleteFile       7
#define opCode_CreatePath       8
#define opCode_DeletePath       9
#define opCode_ServerExtend     127
#define opCode_RequestDir       255

//服务器返回的请求状态码
#define stCode_Undefined            0
#define stCode_CreateUserSucceed    1
#define stCode_UserExist            2
#define stCode_DeleteUserSucceed    3
#define stCode_DeleteUserPWDFail    4
#define stCode_ChangePWD            5
#define stCode_ChangePWDFail        6
#define stCode_LoginSucceed         7
#define stCode_UserUnexist          8
#define stCode_LoginPWDError        9
#define stCode_UploadSucceed        10
#define stCode_DownloadReturn       11
#define stCode_FileUnexist          12
#define stCode_DeleteFileSucceed    13
#define stCode_ShearchFileFailed    14
#define stCode_PathCreateSucceed    15
#define stCode_PathExist            16
#define stCode_PathDeleteSucceed    17
#define stCode_PathUnexist          18
#define stCode_PermissionDenied     19
#define stCode_LoginKeyOutOfData    20
#define stCode_GetFileTreeSucceed   21
#define stCode_GetFileTreeFail      22
#define stCode_ServerExtendSucceed  127
#define stCode_OK                   255


//服务器内部交流代码，与客户端无关
#define svReturn_seccess        0
#define svReturn_UnknowError    1
#define svReturn_FileUnexist    2
#define svReturn_OpUnexist      3
