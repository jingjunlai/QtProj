#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dialogecgset.h"
#include "dialogibpset.h"
#include "dialogrespset.h"
#include "dialogwaveset.h"
#include "dialogparaset.h"
#include "dialogspo2set.h"
#include "dialogarrset.h"

#include "mythreadserial.h"
#include "net/mytcpserver.h"
#include "model/model.h"
#include "model/gpsmodel.h"
#include "labelwave.h"
#include "module/moduledata.h"
#include "chartecg.h"

#include "manager/messagequeue.h"

#include <QMainWindow>
#include <QTimer>
#include <QThread>
#include <windef.h>

namespace Ui {
class MainWindow;
}

#define PARAM_UPDATE_TIME       500
#define STATUS_UPDATE_TIME      500

typedef enum
{
    NullCommand = 0,
    StartNibp,
    StopNibp,
    FirstOverTest,
    SecondOverTest,
    StaticPress,
    AirLeak,
    CalPress
} nibpCommand;

typedef void (*DLL_DisplayNIBPEx)();
typedef void (*DLL_SetNIBPValues)(DWORD press, DWORD pluseWave);
typedef void (*DLL_SetNIBPStart)(bool value);
typedef void (*DLL_SetNIBPMsgHwnd)(HWND hWnd);
typedef void (*DLL_SetNIBPIndex)(int index);
typedef void (*DLL_SetNIBPCaption)();
typedef void (*DLL_DisplayNIBPExOnPos)(int x, int y, int w, int h);
typedef bool (*DLL_GetWindowVisible)();

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    typedef enum
    {
        SerialID = 0,
        ServerID,
        ModelID,
        ID_MAX
    } WorkID;

    typedef enum
    {
        SetGain = 0,
        SetSpeed,

    } ConfigType;


    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();



private slots:

    /*!
     * \brief enableWork ： 等待串口打开后启动工作。
     */
    void enableWork(int id);

    /*!
     * \brief on_pushButton_clicked ：
     */
    void on_pushButtonEcgSet_clicked();

    /*!
     * \brief on_pushButtonStart_clicked ：
     */
    void on_pushButtonStart_clicked();

    void on_pushButtonExit_clicked();

//    void dispPortCnt(unsigned int uiSize);

    void handleDataFromModel(LabelID label, const QVariant &value);

    void handleConfigFromModel(LabelID label, int id, const QVariant *pvalue);

    void updateParam();

    void drawWave();

    void recClientId(int id);

    void disconnectClient(int id);

    void on_pushButtonNibpDisp_clicked();

    void on_pushButtonIbpSet_clicked();

    void on_pushButtonRespSet_clicked();

    void on_pushButtonWaveSet_clicked();

    void on_pushButtonParaSet_clicked();

    void changeWaveLead(DialogWaveSet::LeadMode mode, quint32 param);

    void changeMainConfig(LabelID id, int param1, int param2);

    void on_pushButtonNibpSend_clicked();

    void on_pushButtonSpo2Set_clicked();

    void on_pushButtonCo2Set_clicked();

    void setUserType(int type);



    void on_pushButtonFreeze_clicked();

signals:

    void startThread();

    void getModuleData();

    void sendCommand(int id, const QSharedPointer<ICmsData> data);


private:
    void initDisp();

    void initPort();

    void initModel();

    void initServer();

    bool detectPort();

    int detectNet();

    void closeThread(QThread * const &pThread);

    void startWork();

    void stopWork();

    void resetParamDisp();

    HWND                            m_hWnd;
    QWidget                         *m_NibpDispWidget = Q_NULLPTR;

    DLL_DisplayNIBPEx               m_displayNibp = Q_NULLPTR;
    DLL_SetNIBPValues               m_setNibpValues = Q_NULLPTR;
    DLL_SetNIBPStart                m_setNibpStart = Q_NULLPTR;
    DLL_SetNIBPMsgHwnd              m_setNibpMsgHwnd = Q_NULLPTR;
    DLL_SetNIBPIndex                m_setNibpIndex = Q_NULLPTR;
    DLL_SetNIBPCaption              m_setNibpCaption = Q_NULLPTR;
    DLL_DisplayNIBPExOnPos          m_displayNibpExOnPos = Q_NULLPTR;
    DLL_GetWindowVisible            m_getWindowVisible = Q_NULLPTR;


    Ui::MainWindow                  *ui;
    DialogECGSet                    *m_pDialogEcgSet;
    DialogRespSet                   *m_pDialogRespSet;
    DialogIBPSet                    *m_pDialogIbpSet;
    DialogParaSet                   *m_pDialogParaSet;
    DialogWaveSet                   *m_pDialogWaveSet;
    DialogSPO2Set                   *m_pDialogSpo2Set;
    DialogARRSet                    *m_pDialogCo2Set;
    DialogWaveSet::LeadMode         m_LeadMode = DialogWaveSet::Default;

    MessageQueue                    *m_pMessageQueue = Q_NULLPTR;

    QThread                         *m_pPortThread;
    MySerialPort                    *m_pSerialport = Q_NULLPTR;
    QSerialPort                     *m_pSerial = Q_NULLPTR;
    QThread                         *m_pModelThread;
    Model                           *m_pModel = Q_NULLPTR;
    QThread                         *m_pServerThread;
    MyTcpServer                     *m_pMyServer = Q_NULLPTR;   
    LabelWave                       *m_pWave[LabelWave::MAX_Wave];
    QList<ModuleData *>             m_ModuleDataList;
    QTimer                          *m_pParamTimer;
    QTimer                          *m_pWaveTimer;
    QTimer                          *m_pGetModuleDataTimer;

    int                             m_iClientId = INVALID_DATA;    //暂时只作用一个Client ID。
    bool                            bWorkEnable[ID_MAX] = {false};


    int                             m_iIbpCnt = 0;
    LabelID                         m_iIbpLabel[4] = {LABEL_NULL};
    int                             m_iTempCnt = 0;
    LabelID                         m_iTempLabel[2] = {LABEL_NULL};

    ChartEcg                        *m_pChartEcg;
};

#endif // MAINWINDOW_H
