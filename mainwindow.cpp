#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "module/moduleecg.h"
#include "module/modulespo2.h"
#include "module/moduleresp.h"
#include "module/moduleibp.h"
#include "module/modulenibp.h"
#include "module/moduletemp.h"
#include "module/moduleco2.h"
#include "module/moduleco.h"
#include "labelecgwave.h"
#include "labelspo2wave.h"
#include "labelco2wave.h"
#include "labelibpwave.h"
#include "labelrespswave.h"
#include "labelrespwave.h"
#include "labelnibpwave.h"
#include "net/cmsnibpcommand.h"

#include "port/portmanager.h"

#include <QMessageBox>
#include <QHostInfo>
#include <QLibrary>
#include <QSettings>

const QString EcgLead[12] = {"I", "II", "III", "AVR", "AVL", "AVF", "V1", "V2", "V3", "V4", "V5", "V6"};
const QString EcgSensorCnt[10] = {"RA", "RL", "LA", "LL", "C1", "C2", "C3", "C4", "C5", "C6"};
const QString EcgLeadType[4] = {"3 lead", "5 lead", "6 lead", "12 lead"};
const QString EcgGain[LabelWave::MAX_GAIN] = {"0.25X", "0.5X", "1X", "2X", "4X"};
const QString EcgWorkMode[Gps::ECG_ModeMax] = {"Monitor", "Operation", "Diagnose", "ST", "User"};

const QString NibpMode[14] = {"manual", "period", "stat", "hw over press", "static press", "leak detection", \
                                "venipuncture", "selfcheck", "sw over press", "debug", "auxiliarycycle", "unknown"};

const QString IbpLabel[15] = {"P1", "P2", "ART", "CVP", "PA", "RAP", "LAP", "ICP", "PAWP", "UAP", "BAP", "FAP", \
                                "UVP", "LV", "AO"};

const QString NibpErr[Gps::Nibp_Err_MAX] = {"NoErr","NoDefine","SelfTestFailed","NoErr","NoDefine","NoDefine","LooseCuff","AirLeakage", \
                                           "AirPressureErr","WeakSignal","RangeExceeded","ExcessiveMotion","OverPressure","SignalSaturated", \
                                           "AirSystemLeak","SystemFailure","NoDefine10","NoDefine11","NoDefine12","TimeOut","CuffTypeErr", \
                                           "ZeroFailed","Cal Failed","HardOverPressZeroFailed","HardOverPressCaliFailed"};

const QString TempLabel[5] = {"TEMPA","TEMPB","TEMPC","TEMPD","TEMPE"};
const QString TempStatusLabel[(int)TempStatus::MeasureStatus::Max] = {"MeasureValid","Sensoroff","OverDownLimit","OverUpLimit"};

const QString UserType[Gps::Patient_Max] = {"Adult", "Ped", "Neonate"};
const QString LabelWaveDisc[LabelWave::MAX_Wave] = {"I", "II", "III", "AVR", "AVL", "AVF", "V1", "V2", "V3", "V4", "V5", "V6", "SPO2", \
                                                    "CO2", "IBP1", "IBP2", "IBP3", "IBP4", "RESP", "RESP_SPO2", "NIBP"};

extern quint16   g_NibpWaveValue;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*关闭顶部栏*/
    this->setWindowFlags(Qt::FramelessWindowHint);
    /*背景颜色全黑*/
    this->setStyleSheet("QMainWindow{background:black;}");

    initDisp();
    /*检测串口*/
    detectPort();
    /*检测网络*/
    int cnt = detectNet();
    if(cnt > 0)
        ui->pushButtonStart->setEnabled(true);

    m_pParamTimer = new QTimer(this);
    connect(m_pParamTimer, SIGNAL(timeout()), this, SLOT(updateParam()));

    m_pWaveTimer = new QTimer(this);
    connect(m_pWaveTimer, SIGNAL(timeout()), this, SLOT(drawWave()));

    m_pGetModuleDataTimer = new QTimer(this);

    /*load Nibp dll*/
    QLibrary NibpLib("NIBPDisplay.dll");
    if(NibpLib.load())
    {
        m_displayNibp = (DLL_DisplayNIBPEx)NibpLib.resolve("DisplayNIBPEx");
        m_setNibpValues = (DLL_SetNIBPValues)NibpLib.resolve("SetNIBPValues");
        m_setNibpMsgHwnd = (DLL_SetNIBPMsgHwnd)NibpLib.resolve("SetNIBPMsgHwnd");
        m_setNibpStart = (DLL_SetNIBPStart)NibpLib.resolve("SetNIBPStart");
        m_setNibpIndex = (DLL_SetNIBPIndex)NibpLib.resolve("SetNIBPIndex");
        m_setNibpCaption = (DLL_SetNIBPCaption)NibpLib.resolve("SetNIBPCaption");
        m_displayNibpExOnPos = (DLL_DisplayNIBPExOnPos)NibpLib.resolve("DisplayNIBPExOnPos");
        m_getWindowVisible = (DLL_GetWindowVisible)NibpLib.resolve("GetWindowVisible");
    }

    m_pMessageQueue = new MessageQueue();

    QThread *th = new QThread();
    portManager->setMessageQueue(m_pMessageQueue);
    portManager->moveToThread(th);
    portManager->start();
    th->start();

//    int et = QEvent::registerEventType(EV_Init);
//    CustomEvent *e = new CustomEvent((QEvent::Type)et);
//    QApplication::postEvent(portManager, e);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enableWork(int id)
{
    if(id >= ID_MAX)
    {
        qDebug() << "work id max err";
        return;
    }
    bWorkEnable[id] = true;

    /*串口 or 网络 + 模型才启动工作*/
    if((bWorkEnable[SerialID] && bWorkEnable[ModelID]) || (bWorkEnable[ServerID] && bWorkEnable[ModelID]))
    {
        startWork();
    }
}

void MainWindow::startWork()
{
    resetParamDisp();

    ui->comboBoxModuleType->setEnabled(false);
    ui->comboBoxComPort->setEnabled(false);
    ui->comboBoxBaudrate->setEnabled(false);
    ui->comboBoxIP->setEnabled(false);
    ui->lineEditNetPort->setEnabled(false);
    ui->pushButtonStart->setText(QString("Stop"));
    ui->pushButtonStart->setEnabled(true);
    ui->pushButtonWaveSet->setEnabled(true);
    ui->pushButtonFreeze->setEnabled(true);

    m_pWave[LabelWave::I] = new LabelEcgWave(Gps::ECG_LEAD_I, ui->groupBoxWave_1, 630, 150);
    m_pWave[LabelWave::II] = new LabelEcgWave(Gps::ECG_LEAD_II, ui->groupBoxWave_2, 630, 150);
    m_pWave[LabelWave::III] = new LabelEcgWave(Gps::ECG_LEAD_III, ui->groupBoxWave_3, 630, 150);
    m_pWave[LabelWave::AVR] = new LabelEcgWave(Gps::ECG_LEAD_AVR, ui->groupBoxWave_4, 630, 150);
    m_pWave[LabelWave::AVL] = new LabelEcgWave(Gps::ECG_LEAD_AVL, ui->groupBoxWave_5, 630, 150);
    m_pWave[LabelWave::AVF] = new LabelEcgWave(Gps::ECG_LEAD_AVF, ui->groupBoxWave_6, 630, 150);
    m_pWave[LabelWave::V1] = new LabelEcgWave(Gps::ECG_LEAD_V_1, ui->groupBoxWave_3, 630, 150);
    m_pWave[LabelWave::V2] = new LabelEcgWave(Gps::ECG_LEAD_V_2, ui->groupBoxWave_4, 630, 150);
    m_pWave[LabelWave::V3] = new LabelEcgWave(Gps::ECG_LEAD_V_3, ui->groupBoxWave_9, 630, 150);
    m_pWave[LabelWave::V4] = new LabelEcgWave(Gps::ECG_LEAD_V_4, ui->groupBoxWave_10, 630, 150);
    m_pWave[LabelWave::V5] = new LabelEcgWave(Gps::ECG_LEAD_V_5, ui->groupBoxWave_11, 630, 150);
    m_pWave[LabelWave::V6] = new LabelEcgWave(Gps::ECG_LEAD_V_6, ui->groupBoxWave_12, 630, 150);

    m_pWave[LabelWave::Spo2] = new LabelSpo2Wave(ui->groupBoxWave_5, 630, 150);
    m_pWave[LabelWave::Co2] = new LabelCo2Wave(ui->groupBoxWave_6, 630, 150);
    m_pWave[LabelWave::Ibp1] = new LabelIbpWave(0, ui->groupBoxWave_7, 630, 150);
    m_pWave[LabelWave::Ibp2] = new LabelIbpWave(1, ui->groupBoxWave_8, 630, 150);
    m_pWave[LabelWave::Ibp3] = new LabelIbpWave(2, ui->groupBoxWave_9, 630, 150);
    m_pWave[LabelWave::Ibp4] = new LabelIbpWave(3, ui->groupBoxWave_10, 630, 150);
    m_pWave[LabelWave::Resp] = new LabelRespWave(ui->groupBoxWave_11, 630, 150);
    m_pWave[LabelWave::Resp_s] = new LabelRespSWave(ui->groupBoxWave_12, 630, 150);
    m_pWave[LabelWave::Nibp] = new LabelNibpWave(NULL, 630, 150);

    m_pWave[LabelWave::I]->setWaveEnabled(true);
    m_pWave[LabelWave::II]->setWaveEnabled(true);
    m_pWave[LabelWave::V1]->setWaveEnabled(true);
    m_pWave[LabelWave::V2]->setWaveEnabled(true);
    m_pWave[LabelWave::Spo2]->setWaveEnabled(true);
    m_pWave[LabelWave::Co2]->setWaveEnabled(true);
    m_pWave[LabelWave::Ibp1]->setWaveEnabled(true);
    m_pWave[LabelWave::Ibp2]->setWaveEnabled(true);
    m_pWave[LabelWave::Ibp3]->setWaveEnabled(true);
    m_pWave[LabelWave::Ibp4]->setWaveEnabled(true);
    m_pWave[LabelWave::Resp]->setWaveEnabled(true);
    m_pWave[LabelWave::Resp_s]->setWaveEnabled(true);

    m_pWave[LabelWave::Spo2]->setLimit(255);
    m_pWave[LabelWave::Ibp1]->setLimit(225);
    m_pWave[LabelWave::Ibp2]->setLimit(225);
    m_pWave[LabelWave::Ibp3]->setLimit(225);
    m_pWave[LabelWave::Ibp4]->setLimit(225);

    for(int i = 0; i< LabelWave::MAX_Wave; i++)
    {
        m_pWave[i]->move(5,5);
        m_pWave[i]->lower();
    }

    QSettings *configIni = new QSettings("params.ini", QSettings::IniFormat);
    ui->labelEcgGain->setText(EcgGain[configIni->value("Gain/Ecg").toInt()]);
    for(int i = LabelWave::Ecg_Start; i< LabelWave::Ecg_End + 1; i++)
    {
        m_pWave[i]->setGain((LabelWave::WaveGain)configIni->value("Gain/Ecg").toInt());
        m_pWave[i]->setSpeed((LabelWave::WaveSpeed)configIni->value("WaveSpeed/Ecg").toInt());
    }
    m_pWave[LabelWave::Spo2]->setSpeed((LabelWave::WaveSpeed)configIni->value("WaveSpeed/Spo2").toInt());
    m_pWave[LabelWave::Co2]->setSpeed((LabelWave::WaveSpeed)configIni->value("WaveSpeed/Co2").toInt());
    for(int i = LabelWave::Ibp_Start; i < LabelWave::Ibp_End + 1; i++)
    {
        m_pWave[i]->setSpeed((LabelWave::WaveSpeed)configIni->value("WaveSpeed/Ibp").toInt());
    }
    m_pWave[LabelWave::Resp]->setSpeed((LabelWave::WaveSpeed)configIni->value("WaveSpeed/Resp").toInt());
    m_pWave[LabelWave::Resp_s]->setSpeed((LabelWave::WaveSpeed)configIni->value("WaveSpeed/RespS").toInt());

    m_pDialogEcgSet = new DialogECGSet(this);
    m_pDialogIbpSet = new DialogIBPSet(this);
    m_pDialogRespSet = new DialogRespSet(this);
    m_pDialogWaveSet = new DialogWaveSet(this);
    m_pDialogParaSet = new DialogParaSet(this);
    m_pDialogSpo2Set = new DialogSPO2Set(this);
    m_pDialogCo2Set = new DialogARRSet(this);

    m_pDialogEcgSet->setServerPointer(m_pMyServer);
    m_pDialogIbpSet->setServerPointer(m_pMyServer);
    m_pDialogRespSet->setServerPointer(m_pMyServer);

    m_pDialogEcgSet->setWaveSpeed((LabelWave::WaveSpeed)configIni->value("WaveSpeed/Ecg").toInt());
    m_pDialogSpo2Set->setSpo2WaveSpeed((LabelWave::WaveSpeed)configIni->value("WaveSpeed/Spo2").toInt());
    m_pDialogSpo2Set->setRespWaveSpeed((LabelWave::WaveSpeed)configIni->value("WaveSpeed/RespS").toInt());
    m_pDialogCo2Set->setWaveSpeed((LabelWave::WaveSpeed)configIni->value("WaveSpeed/Co2").toInt());
    m_pDialogIbpSet->setWaveSpeed((LabelWave::WaveSpeed)configIni->value("WaveSpeed/Ibp").toInt());
    m_pDialogRespSet->setWaveSpeed((LabelWave::WaveSpeed)configIni->value("WaveSpeed/Resp").toInt());
    delete configIni;

    m_pDialogWaveSet->setWaveLead(DialogWaveSet::Default);
    changeWaveLead(DialogWaveSet::Default, 0);
    for(int i = LabelWave::Ecg_Start; i < LabelWave::Ecg_End + 1; i++)
        m_pDialogEcgSet->setWavePointer(m_pWave[i]);
    for(int i = LabelWave::Ibp_Start; i < LabelWave::Ibp_End + 1; i++)
        m_pDialogIbpSet->setWavePointer(m_pWave[i]);
    m_pDialogRespSet->setWavePointer(m_pWave[LabelWave::Resp]);
    m_pDialogSpo2Set->setWavePointer(m_pWave[LabelWave::Spo2]);
    m_pDialogSpo2Set->setWavePointer(m_pWave[LabelWave::Resp_s]);
    m_pDialogCo2Set->setWavePointer(m_pWave[LabelWave::Co2]);

    m_pParamTimer->start(PARAM_UPDATE_TIME);
    m_pWaveTimer->start(DRAW_TIMER_CNT);

    /*定时获取各个模块的数据指针*/
    m_pGetModuleDataTimer->start(1000);
}

void MainWindow::stopWork()
{
    ui->labelStatus->setText("Disconnect");
    ui->labelStatus->setStyleSheet("color:red;");
    ui->pushButtonEcgSet->setEnabled(false);
    ui->pushButtonIbpSet->setEnabled(false);
    ui->pushButtonNibpDisp->setEnabled(false);
    ui->pushButtonSpo2Set->setEnabled(false);
    ui->pushButtonCo2Set->setEnabled(false);
    ui->pushButtonWaveSet->setEnabled(false);
    ui->pushButtonFreeze->setEnabled(false);
    ui->pushButtonRespSet->setEnabled(false);
    ui->pushButtonNibpSend->setEnabled(false);
    ui->comboBoxNibpCommand->setEnabled(false);
    ui->lineEditCalPress->setEnabled(false);

    m_pParamTimer->stop();
    m_pWaveTimer->stop();
    m_pGetModuleDataTimer->stop();

    m_pDialogEcgSet->deleteLater();
    m_pDialogIbpSet->deleteLater();
    m_pDialogRespSet->deleteLater();
    m_pDialogWaveSet->deleteLater();
    m_pDialogParaSet->deleteLater();
    m_pDialogSpo2Set->deleteLater();
    m_pDialogCo2Set->deleteLater();

    for(int i = 0; i < LabelWave::MAX_Wave; i++)
    {
        m_pWave[i]->deleteLater();
    }

    if(m_pSerialport != Q_NULLPTR)
    {
        m_pSerialport->deleteLater();
        m_pSerialport = Q_NULLPTR;
    }

    if(m_pModel != Q_NULLPTR)
    {
        m_pModel->deleteLater();
        m_pModel = Q_NULLPTR;
    }

    if(m_pMyServer != Q_NULLPTR)
    {
        m_pMyServer->deleteLater();
        m_pMyServer = Q_NULLPTR;
    }

    closeThread(m_pPortThread);
    closeThread(m_pModelThread);
    closeThread(m_pServerThread);

    memset(&bWorkEnable, 0, sizeof(bWorkEnable));
    m_ModuleDataList.clear();
    m_iIbpCnt = 0;
    m_iTempCnt = 0;

    ui->comboBoxIP->setEnabled(true);
    ui->lineEditNetPort->setEnabled(true);
    ui->comboBoxModuleType->setEnabled(true);
    ui->comboBoxComPort->setEnabled(true);
    ui->comboBoxBaudrate->setEnabled(true);
    ui->pushButtonStart->setText(QString("Start"));
}

void MainWindow::resetParamDisp()
{
    ui->labelHR->setText("---");
    ui->labelEcgWorkMode->setText("Work Mode");
    ui->labelEcgHUM->setText("NULL");
    ui->labelEcgEMG->setText("NULL");
    ui->labelEcgDriftDisc->setText("DRIFT:");
    ui->labelEcgDrift->setText("---");
    ui->labelEcgSensor->show();
    ui->labelEcgPackets->setText("----/s");
    ui->labelEcgSensorCnt->setText("");
    ui->labelEcgSat->setText("---");
    ui->labelEcgPvcs->setText("---");
    ui->labelEcgCalLead->setText("--");
    ui->labelEcgMode->setText("NULL");

    ui->labelSpo2Value->setText("---");
    ui->labelSpo2Pr->setText("---");
    ui->labelSpo2Rr->setText("---");
    ui->labelSpo2Pi->setText("---");
    ui->labelSpo2Packets->setText("---/s");
    ui->labelSpo2Sensor->show();
    ui->labelSpo2Sensoroff->show();

    ui->labelNibpWork->setText("stop");
    ui->labelNibpMode->setText("NULL");
    ui->labelNibpPackets->setText("---/s");
    ui->labelNibpErr->setText("NULL");
    ui->labelNibpValue->setText("---/---(---)");
    ui->labelNibpPr->setText("---");

    ui->labelRespPackets->setText("---/s");
    ui->labelRespSensor->show();
    ui->labelRespValue->setText("---");

    ui->labelCo2Packets->setText("---/s");
    ui->labelRespSensor->show();
    ui->labelCo2Value->setText("---");

    ui->labelIbp1Sensor->show();
    ui->labelIbp2Sensor->show();
    ui->labelIbp3Sensor->show();
    ui->labelIbp4Sensor->show();
    ui->labelIbp1WaveID->setText("NULL");
    ui->labelIbp2WaveID->setText("NULL");
    ui->labelIbp3WaveID->setText("NULL");
    ui->labelIbp4WaveID->setText("NULL");
    ui->labelIbp1ID->setText("NULL");
    ui->labelIbp2ID->setText("NULL");
    ui->labelIbp3ID->setText("NULL");
    ui->labelIbp4ID->setText("NULL");
    ui->labelIbp1Zero->setText("failed");
    ui->labelIbp2Zero->setText("failed");
    ui->labelIbp3Zero->setText("failed");
    ui->labelIbp4Zero->setText("failed");
    ui->labelIbp1Cal->setText("failed");
    ui->labelIbp2Cal->setText("failed");
    ui->labelIbp3Cal->setText("failed");
    ui->labelIbp4Cal->setText("failed");
    ui->labelIbp1Packets->setText("---/s");
    ui->labelIbp2Packets->setText("---/s");
    ui->labelIbp3Packets->setText("---/s");
    ui->labelIbp4Packets->setText("---/s");
    ui->labelIbp1Value->setText("---/---(---)");
    ui->labelIbp2Value->setText("---/---(---)");
    ui->labelIbp3Value->setText("---/---(---)");
    ui->labelIbp4Value->setText("---/---(---)");

    ui->labelTemp1ID->setText("NULL");
    ui->labelTemp2ID->setText("NULL");
    ui->labelTemp1Packets->setText("---/s");
    ui->labelTemp2Packets->setText("---/s");
    ui->labelTempStatus1->setText("NULL");
    ui->labelTempStatus2->setText("NULL");
    ui->labelTemp1Value->setText("---");
    ui->labelTemp2Value->setText("---");

}



void MainWindow::on_pushButtonStart_clicked()
{
    if(!detectPort())
    {
        QMessageBox::about(NULL, "提示", "串口打开失败");
        return;
    }
    if(ui->pushButtonStart->text() == QString("Start"))
    {
        ui->pushButtonStart->setEnabled(false);
//        initPort();

        SerialSetting set(ui->comboBoxComPort->currentText(), \
                          ui->comboBoxBaudrate->currentText().toInt(), \
                          QSerialPort::Data8, \
                          QSerialPort::NoParity, \
                          QSerialPort::OneStop);
        portManager->setSerialSetting(set);


        VariantEv *ev = new VariantEv((QEvent::Type)EV_Init);
        m_pMessageQueue->publish(EV_Init, ev);

        initServer();
        initModel();
        /*启动线程*/
        emit startThread();


    }
    else
    {
        stopWork();
    }
    qDebug()<<"main thread ID = "<<QThread::currentThreadId();
}

void MainWindow::on_pushButtonExit_clicked()
{
    if(ui->pushButtonStart->text() != QString("Start"))
    {
        stopWork();
    }
    exit(0);
}

//void MainWindow::dispPortCnt(unsigned int uiSize)
//{
//    ui->labelPortCnt->setText(QString::number(uiSize));
//}

void MainWindow::handleDataFromModel(LabelID label, const QVariant &value)
{
    switch(label)
    {
        case LABEL_ECG:
            m_ModuleDataList.append(value.value<EcgModuleData*>());
            m_pWave[LabelWave::I]->setPointer(value.value<EcgModuleData*>());
            ui->pushButtonEcgSet->setEnabled(true);
            break;
        case LABEL_SPO2:
            m_ModuleDataList.append(value.value<Spo2ModuleData*>());
            m_pWave[LabelWave::Spo2]->setPointer(value.value<Spo2ModuleData*>());
            m_pWave[LabelWave::Resp_s]->setPointer(value.value<Spo2ModuleData*>());
            ui->pushButtonSpo2Set->setEnabled(true);
            break;
        case LABEL_NIBP:
            m_ModuleDataList.append(value.value<NibpModuleData*>());
            m_pWave[LabelWave::Nibp]->setPointer(value.value<NibpModuleData*>());
            ui->pushButtonNibpDisp->setEnabled(true);
            ui->pushButtonNibpSend->setEnabled(true);
            ui->comboBoxNibpCommand->setEnabled(true);
            ui->lineEditCalPress->setEnabled(true);
            break;
        case LABEL_RESP:
            m_ModuleDataList.append(value.value<RespModuleData*>());
            m_pWave[LabelWave::Resp]->setPointer(value.value<RespModuleData*>());
            ui->pushButtonRespSet->setEnabled(true);
            break;
        case LABEL_CO2:
            m_ModuleDataList.append(value.value<Co2ModuleData*>());
            m_pWave[LabelWave::Co2]->setPointer(value.value<Co2ModuleData*>());
            ui->pushButtonCo2Set->setEnabled(true);
            break;
        case LABEL_CO:
            m_ModuleDataList.append(value.value<CoModuleData*>());
            break;
        case LABEL_IBP_START ... LABEL_IBP_END:   
            if(m_iIbpCnt < 4)
            {
                m_ModuleDataList.append(value.value<IbpModuleData*>());
                m_pWave[LabelWave::Ibp1 + m_iIbpCnt]->setPointer(value.value<IbpModuleData*>());
                m_iIbpCnt++;
            }
            ui->pushButtonIbpSet->setEnabled(true);
            break;
        case LABEL_TEMP_START ... LABEL_TEMP_END:
            if(m_iTempCnt < 2)
            {
                m_ModuleDataList.append(value.value<TempModuleData*>());
                m_iTempCnt++;
            }
            break;
        default:
            break;
    }
}

void MainWindow::handleConfigFromModel(LabelID label, int id, const QVariant *pvalue)
{
    qDebug() << "主界面接收到 配置 Label ID = " << label << "id = " << id;


    delete pvalue;
}

void MainWindow::updateParam()
{
    /*定时500ms，遍历所有的波形，参数，状态，配置信息*/
    for(int i = 0; i != m_ModuleDataList.size(); i++)
    {
        switch(m_ModuleDataList[i]->labelId())
        {
            case LABEL_ECG:
                {
                    auto v = dynamic_cast<EcgModuleData *>(m_ModuleDataList[i]);

                    ui->labelHR->setText((v->m_pParam->hr == INVALID_DATA? "---" : QString::number(v->m_pParam->hr)));
                    ui->labelEcgWorkMode->setText(EcgWorkMode[v->m_pStatus->workMode]);
                    ui->labelEcgHUM->setText("50Hz");
                    switch(v->m_pStatus->workMode)
                    {
                        case Gps::ECG_ModeMonitor:
                            ui->labelEcgEMG->setText("150Hz");
                            ui->labelEcgDriftDisc->setText("DRIFT:");
                            ui->labelEcgDrift->setText("OFF");
                            break;
                        case Gps::ECG_ModeOperation:
                            ui->labelEcgEMG->setText("25Hz");
                            ui->labelEcgDriftDisc->setText("DRIFT2:");
                            ui->labelEcgDrift->setText("ON");
                            break;
                        case Gps::ECG_ModeDiagnose:
                            ui->labelEcgEMG->setText("45Hz");
                            ui->labelEcgDriftDisc->setText("DRIFT1:");
                            ui->labelEcgDrift->setText("ON");
                            break;
                        case Gps::ECG_ModeST:
                            ui->labelEcgEMG->setText("45Hz");
                            ui->labelEcgDriftDisc->setText("DRIFT:");
                            ui->labelEcgDrift->setText("OFF");
                            break;
                        default:
                            break;
                    }
                    v->m_pStatus->sensorOff? ui->labelEcgSensor->show() : ui->labelEcgSensor->hide();
                    ui->labelEcgPackets->setText(QString::number(v->m_ModuleInfo.packets.WavePackets) + "/s");
                    QString s = "";
                    for(int i = 0; i < Gps::EcgElecMax; i++)
                        s += (v->m_pStatus->electrodeOff[i]? (EcgSensorCnt[i] + " ") : (""));
                    ui->labelEcgSensorCnt->setText(s);
                    QString s1 = "";
                    for(int i = 0; i < Gps::ECG_LEAD_MAX; i++)
                        s1 += (v->m_pStatus->electrodePolar[i]? (EcgLead[i] + " ") : (""));                  
                    ui->labelEcgSat->setText(s1 == ""? "---" : s1);
                    ui->labelEcgPvcs->setText(v->m_pParam->pvcCnt == INVALID_DATA? "---" : QString::number(v->m_pParam->pvcCnt));
                    ui->labelEcgCalLead->setText(EcgLead[v->m_pStatus->calLead]);
                    ui->labelEcgMode->setText(EcgLeadType[v->m_pStatus->leadType]);
                }
                break;
            case LABEL_SPO2:
                {
                    auto v = dynamic_cast<Spo2ModuleData *>(m_ModuleDataList[i]);

                    ui->labelSpo2Value->setText((v->m_pParam->m_iSpO2Value == INVALID_DATA? "---" : QString::number(v->m_pParam->m_iSpO2Value)));
                    ui->labelSpo2Pr->setText((v->m_pParam->m_iPrValue == INVALID_DATA? "---" : QString::number(v->m_pParam->m_iPrValue)));
                    ui->labelSpo2Rr->setText((v->m_pParam->m_iBreathRate == INVALID_DATA? "---" : QString::number(v->m_pParam->m_iBreathRate)));
                    ui->labelSpo2Pi->setText((v->m_pParam->m_iPIValue == INVALID_DATA? "---" : QString::number((float)v->m_pParam->m_iPIValue / 100) + " %"));
                    ui->labelSpo2Packets->setText(QString::number(v->m_ModuleInfo.packets.WavePackets) + "/s");

                    v->m_pStatus->m_iProbeLose == 0? ui->labelSpo2Sensor->hide() : ui->labelSpo2Sensor->show();
                    v->m_pStatus->m_iSensorOff == 0? ui->labelSpo2Sensoroff->hide() : ui->labelSpo2Sensoroff->show();
                }
                break;
            case LABEL_NIBP:
                {
                    auto v = dynamic_cast<NibpModuleData *>(m_ModuleDataList[i]);

                    ui->labelNibpWork->setText(v->m_pStatus->isWorking == 0? "stop" : "working");
                    ui->labelNibpMode->setText(NibpMode[v->m_pStatus->workMode]);
                    ui->labelNibpPackets->setText(QString::number(v->m_ModuleInfo.packets.WavePackets) + "/s");
                    QString err;
                    err = QString("E%1").arg(v->m_pStatus->errorCode, 2, 16, QLatin1Char('0')) + ":" + NibpErr[v->m_pStatus->errorCode];
                    ui->labelNibpErr->setText(err);
                    if(v->m_pParam->isValid())
                    {
                        ui->labelNibpValue->setText(QString::number(v->m_pParam->sys) + "/" + QString::number(v->m_pParam->dia) + "(" + QString::number(v->m_pParam->map) + ")");
                        ui->labelNibpPr->setText(QString::number(v->m_pParam->pr));
                    }
                    else
                    {
                        ui->labelNibpValue->setText("---/---(---)");
                        ui->labelNibpPr->setText("---");
                    }
                }
                break;
            case LABEL_RESP:
                {
                    auto v = dynamic_cast<RespModuleData *>(m_ModuleDataList[i]);
                    ui->labelRespPackets->setText(QString::number(v->m_ModuleInfo.packets.WavePackets) + "/s");
                    v->m_pStatus->m_LeadsOff == 0? ui->labelRespSensor->hide() : ui->labelRespSensor->show();
                    ui->labelRespValue->setText((v->m_pParam->m_RespRate == INVALID_DATA? "---" : QString::number(v->m_pParam->m_RespRate)));
                }
                break;
            case LABEL_CO2:
                {
                    auto v = dynamic_cast<Co2ModuleData *>(m_ModuleDataList[i]);

                    ui->labelCo2Packets->setText(QString::number(v->m_ModuleInfo.packets.WavePackets) + "/s");
                    v->m_pStatus->sensorOff == 0? ui->labelRespSensor->hide() : ui->labelRespSensor->show();
                    ui->labelCo2Value->setText((v->m_pParam->breathRate == INVALID_DATA? "---" : QString::number(v->m_pParam->breathRate)));
                }
                break;

            case LABEL_IBP_START ... LABEL_IBP_END:
                {
                    /*自动根据接收到Label生成IBP板块，最多4个*/
                    auto v = dynamic_cast<IbpModuleData *>(m_ModuleDataList[i]);
                    int cnt = 0;
                    for(; cnt < 4; cnt++)
                    {
                        if(m_iIbpLabel[cnt] == LABEL_NULL)
                        {
                            m_iIbpLabel[cnt] = v->labelId();
                            m_pDialogIbpSet->setLabel((CmsLabelID)v->labelId(), cnt);
                            break;
                        }
                        else if(m_iIbpLabel[cnt] == v->labelId())
                        {
                            break;
                        }

                    }
                    switch(cnt)
                    {
                        case 0:
                            {
                                v->m_pStatus->sensorOff == 0? ui->labelIbp1Sensor->hide() : ui->labelIbp1Sensor->show();
                                ui->labelIbp1WaveID->setText(IbpLabel[v->labelId() - LABEL_IBP_START]);
                                ui->labelIbp1ID->setText(IbpLabel[v->labelId() - LABEL_IBP_START]);
                                ui->labelIbp1Zero->setText(v->m_pStatus->zeroState == 0? "success" : "failed");
                                ui->labelIbp1Cal->setText(v->m_pStatus->calibrationState == 0? "success" : "failed");
                                ui->labelIbp1Packets->setText(QString::number(v->m_ModuleInfo.packets.WavePackets) + "/s");
                                if(v->m_pParam->isValid())
                                {
                                    ui->labelIbp1Value->setText(QString::number(v->m_pParam->sys) + "/" + QString::number(v->m_pParam->dia) + "(" + QString::number(v->m_pParam->mean) + ")");
                                }
                                else
                                    ui->labelIbp1Value->setText("---/---(---)");
                            }
                            break;
                        case 1:
                            {
                                v->m_pStatus->sensorOff == 0? ui->labelIbp2Sensor->hide() : ui->labelIbp2Sensor->show();
                                ui->labelIbp2WaveID->setText(IbpLabel[v->labelId() - LABEL_IBP_START]);
                                ui->labelIbp2ID->setText(IbpLabel[v->labelId() - LABEL_IBP_START]);
                                ui->labelIbp2Zero->setText(v->m_pStatus->zeroState == 0? "success" : "failed");
                                ui->labelIbp2Cal->setText(v->m_pStatus->calibrationState == 0? "success" : "failed");
                                ui->labelIbp2Packets->setText(QString::number(v->m_ModuleInfo.packets.WavePackets) + "/s");
                                if(v->m_pParam->isValid())
                                {
                                    ui->labelIbp2Value->setText(QString::number(v->m_pParam->sys) + "/" + QString::number(v->m_pParam->dia) + "(" + QString::number(v->m_pParam->mean) + ")");
                                }
                                else
                                    ui->labelIbp2Value->setText("---/---(---)");

                            }
                            break;
                        case 2:
                            {
                                v->m_pStatus->sensorOff == 0? ui->labelIbp3Sensor->hide() : ui->labelIbp3Sensor->show();
                                ui->labelIbp3WaveID->setText(IbpLabel[v->labelId() - LABEL_IBP_START]);
                                ui->labelIbp3ID->setText(IbpLabel[v->labelId() - LABEL_IBP_START]);
                                ui->labelIbp3Zero->setText(v->m_pStatus->zeroState == 0? "success" : "failed");
                                ui->labelIbp3Cal->setText(v->m_pStatus->calibrationState == 0? "success" : "failed");
                                ui->labelIbp3Packets->setText(QString::number(v->m_ModuleInfo.packets.WavePackets) + "/s");
                                if(v->m_pParam->isValid())
                                {
                                    ui->labelIbp3Value->setText(QString::number(v->m_pParam->sys) + "/" + QString::number(v->m_pParam->dia) + "(" + QString::number(v->m_pParam->mean) + ")");
                                }
                                else
                                    ui->labelIbp3Value->setText("---/---(---)");

                            }
                            break;
                        case 3:
                            {
                                v->m_pStatus->sensorOff == 0? ui->labelIbp4Sensor->hide() : ui->labelIbp4Sensor->show();
                                ui->labelIbp4WaveID->setText(IbpLabel[v->labelId() - LABEL_IBP_START]);
                                ui->labelIbp4ID->setText(IbpLabel[v->labelId() - LABEL_IBP_START]);
                                ui->labelIbp4Zero->setText(v->m_pStatus->zeroState == 0? "success" : "failed");
                                ui->labelIbp4Cal->setText(v->m_pStatus->calibrationState == 0? "success" : "failed");
                                ui->labelIbp4Packets->setText(QString::number(v->m_ModuleInfo.packets.WavePackets) + "/s");
                                if(v->m_pParam->isValid())
                                {
                                    ui->labelIbp4Value->setText(QString::number(v->m_pParam->sys) + "/" + QString::number(v->m_pParam->dia) + "(" + QString::number(v->m_pParam->mean) + ")");
                                }
                                else
                                    ui->labelIbp4Value->setText("---/---(---)");
                            }
                            break;
                        default:
                            qDebug() << "too much IBP label";
                            break;
                    }


                }
                break;

            case LABEL_TEMP_START ... LABEL_TEMP_END:
                {
                    auto v = dynamic_cast<TempModuleData *>(m_ModuleDataList[i]);
                    int cnt = 0;
                    for(; cnt < 2; cnt++)
                    {
                        if(m_iTempLabel[cnt] == LABEL_NULL)
                        {
                            m_iTempLabel[cnt] = v->labelId();
                            break;
                        }
                        else if(m_iTempLabel[cnt] == v->labelId())
                        {
                            break;
                        }
                    }
                    switch(cnt)
                    {
                        case 0:
                            {
                                ui->labelTemp1ID->setText(TempLabel[v->labelId() - LABEL_TEMP_START]);
                                ui->labelTemp1Packets->setText(QString::number(v->m_ModuleInfo.packets.ParamPackets) + "/s");
                                ui->labelTempStatus1->setText(TempStatusLabel[(int)v->m_pStatus->measureStatus[0]]);
                                if(v->m_pParam->data[0].temp == INVALID_DATA)
                                    ui->labelTemp1Value->setText("---");
                                else
                                    ui->labelTemp1Value->setText(QString::number((float)v->m_pParam->data[0].temp / 100));
                            }
                            break;
                        case 1:
                            {
                                ui->labelTemp2ID->setText(TempLabel[v->labelId() - LABEL_TEMP_START]);
                                ui->labelTemp2Packets->setText(QString::number(v->m_ModuleInfo.packets.ParamPackets) + "/s");
                                ui->labelTempStatus2->setText(TempStatusLabel[(int)v->m_pStatus->measureStatus[0]]);
                                if(v->m_pParam->data[0].temp == INVALID_DATA)
                                    ui->labelTemp2Packets->setText("---");
                                else
                                    ui->labelTemp2Packets->setText(QString::number((float)v->m_pParam->data[0].temp / 100));
                            }
                            break;
                    }
                }
                break;
            default:
                {

                }
                break;

        }
    }

}

void MainWindow::drawWave()
{
    for(int i = 0; i < LabelWave::MAX_Wave; i++)
    {
        if(m_pWave[i]->getEnabled())
            m_pWave[i]->drawProcess();
    }

    /*refresh nibp press data*/
    m_pWave[LabelWave::Nibp]->drawProcess();
    ui->labelNibpPress->setText(g_NibpWaveValue == INVALID_DATA? "---" : QString::number(g_NibpWaveValue));
    if(m_setNibpValues != Q_NULLPTR && g_NibpWaveValue != INVALID_DATA)
        m_setNibpValues(g_NibpWaveValue, 0);
}

void MainWindow::recClientId(int id)
{
    m_iClientId = id;
    m_pDialogEcgSet->setClientId(m_iClientId);
    m_pDialogIbpSet->setClientId(m_iClientId);
    m_pDialogRespSet->setClientId(m_iClientId);
    ui->labelStatus->setStyleSheet("color:green;");
    ui->labelStatus->setText("Connected");
}

void MainWindow::disconnectClient(int id)
{
    if(m_iClientId == id)
    {
        m_iClientId = INVALID_DATA;
        m_pDialogEcgSet->setClientId(m_iClientId);
        m_pDialogIbpSet->setClientId(m_iClientId);
        m_pDialogRespSet->setClientId(m_iClientId);
        ui->labelStatus->setStyleSheet("color:red;");
        ui->labelStatus->setText("Disconnect");
    }
}

void MainWindow::initDisp()
{
    ui->groupBoxWave_13->hide();



    ui->labelWave_1->setStyleSheet("color:yellow;");
    ui->labelWave_2->setStyleSheet("color:yellow;");
    ui->labelWave_3->setStyleSheet("color:yellow;");
    ui->labelWave_4->setStyleSheet("color:yellow;");
    ui->labelWave_5->setStyleSheet("color:yellow;");
    ui->labelWave_6->setStyleSheet("color:yellow;");
    ui->labelWave_7->setStyleSheet("color:yellow;");
    ui->labelWave_8->setStyleSheet("color:yellow;");
    ui->labelWave_9->setStyleSheet("color:yellow;");
    ui->labelWave_10->setStyleSheet("color:yellow;");
    ui->labelWave_11->setStyleSheet("color:yellow;");
    ui->labelWave_12->setStyleSheet("color:yellow;");
    ui->labelWave_13->setStyleSheet("color:yellow;");

    ui->labelIbp1WaveID->setStyleSheet("color:yellow;");
    ui->labelIbp2WaveID->setStyleSheet("color:yellow;");
    ui->labelIbp3WaveID->setStyleSheet("color:yellow;");
    ui->labelIbp4WaveID->setStyleSheet("color:yellow;");
    ui->labelIbpWaveID->setStyleSheet("color:yellow;");

    ui->labelModuleType->setStyleSheet("color:yellow;");
    ui->labelComPort->setStyleSheet("color:yellow;");
    ui->labelBaudrate->setStyleSheet("color:yellow;");
    ui->groupBoxCommand->setStyleSheet("QGroupBox{color:yellow;}");
    ui->groupBoxSelect->setStyleSheet("QGroupBox{color:yellow;}");
    ui->groupBoxUserInfo->setStyleSheet("QGroupBox{color:yellow;}");
    ui->labelUser->setStyleSheet("color:red;");
    ui->labelStatus->setStyleSheet("color:red;");
    ui->labelPatientDisc->setStyleSheet("color:yellow;");
    ui->labelStatusDisc->setStyleSheet("color:yellow;");
    ui->groupBoxNibpCommand->setStyleSheet("QGroupBox{color:yellow;}");
    ui->labelNibpCommand->setStyleSheet("color:yellow");
    ui->labelNibpCalPress->setStyleSheet("color:yellow");
    ui->labelNibpmmHg->setStyleSheet("color:yellow");

    ui->groupBoxPara_1->setStyleSheet("color:yellow;");
    ui->groupBoxPara_2->setStyleSheet("color:beige;");
    ui->groupBoxPara_3->setStyleSheet("color:cyan;");
    ui->groupBoxPara_4->setStyleSheet("color:white;");
    ui->groupBoxPara_5->setStyleSheet("color:plum;");
    ui->groupBoxPara_6->setStyleSheet("color:greenyellow;");
    ui->groupBoxPara_7->setStyleSheet("color:greenyellow;");
    ui->groupBoxPara_8->setStyleSheet("color:greenyellow;");
    ui->groupBoxPara_9->setStyleSheet("color:greenyellow;");
    ui->groupBoxPara_10->setStyleSheet("color:indigo;");
    ui->groupBoxPara_11->setStyleSheet("color:lightsteelblue;");

    ui->labelIP->setStyleSheet("color:yellow;");
    ui->labelNetPort->setStyleSheet("color:yellow;");

}

void MainWindow::initPort()
{
    if(m_pSerialport == Q_NULLPTR)
    {
        /*串口线程*/
        m_pSerialport = new MySerialPort(ui->comboBoxComPort->currentText(), ui->comboBoxBaudrate->currentText().toInt());
        m_pPortThread = new QThread;
        m_pSerialport->moveToThread(m_pPortThread);
        connect(this, SIGNAL(startThread()), m_pSerialport, SLOT(doWork()));
        connect(m_pSerialport, SIGNAL(enableMainWork(int)), this, SLOT(enableWork(int)));
        m_pPortThread->start();
    }
}

void MainWindow::initModel()
{
    if(m_pModel == Q_NULLPTR)
    {
        /*模型线程*/
        m_pModel = new GpsModel;
        //m_pModel->setPort(m_pSerialport);
        m_pModelThread = new QThread;
        m_pModel->moveToThread(m_pModelThread);
        if(m_pMyServer != NULL)
        {
            connect(m_pMyServer, SIGNAL(sendDataToModel(QByteArray)), m_pModel, SLOT(handleSocketData(QByteArray)), Qt::QueuedConnection);
            m_pMyServer->setModelPoint(m_pModel);
        }
        /*打印串口接收数量*/
//        connect(m_pModel, SIGNAL(sendPortDataSizeToMain(uint)), this, SLOT(dispPortCnt(uint)));
        qRegisterMetaType<LabelID>("LabelID");

        connect(m_pGetModuleDataTimer, SIGNAL(timeout()), m_pModel, SLOT(sendModuleData()), Qt::QueuedConnection);
//        connect(this, SIGNAL(getModuleData()), m_pModel, SLOT(sendModuleData()), Qt::QueuedConnection);
        connect(m_pModel, SIGNAL(sendUserType(int)), this, SLOT(setUserType(int)), Qt::QueuedConnection);
        connect(m_pModel, SIGNAL(enableMainWork(int)), this, SLOT(enableWork(int)));
        connect(m_pModel, SIGNAL(sendModuleDataToMain(LabelID,const QVariant&)), this, SLOT(handleDataFromModel(LabelID,const QVariant&)), Qt::QueuedConnection);
        connect(m_pModel, SIGNAL(sendConfigToMain(LabelID,int,const QVariant*)), this, SLOT(handleConfigFromModel(LabelID,int,const QVariant*)), Qt::QueuedConnection);
        connect(this, SIGNAL(startThread()), m_pModel, SLOT(doWork()));
        m_pModelThread->start();
    }
}

void MainWindow::initServer()
{
    if(m_pMyServer == Q_NULLPTR)
    {
        m_pMyServer = new MyTcpServer(ui->lineEditNetPort->text().toInt());

        m_pServerThread = new QThread;
        m_pMyServer->moveToThread(m_pServerThread);
        connect(this, SIGNAL(startThread()), m_pMyServer, SLOT(doWork()), Qt::QueuedConnection);
        connect(m_pMyServer, SIGNAL(enableMainWork(int)), this, SLOT(enableWork(int)), Qt::QueuedConnection);
        connect(m_pMyServer, SIGNAL(sigNewClient(int)), this, SLOT(recClientId(int)), Qt::QueuedConnection);
        connect(m_pMyServer, SIGNAL(sigDisconnectClient(int)), this, SLOT(disconnectClient(int)), Qt::QueuedConnection);
        qRegisterMetaType<QSharedPointer<ICmsData>>("QSharedPointer<ICmsData>");
        connect(this, SIGNAL(sendCommand(int, QSharedPointer<ICmsData>)), m_pMyServer, SLOT(transmitCommand(int, const QSharedPointer<ICmsData>)), Qt::QueuedConnection);
        m_pServerThread->start();
    }
}

bool MainWindow::detectPort()
{
    QStringList slt;
    MySerialPort::detectPort(&slt);
    if(slt.count() == 0)
    {
        ui->pushButtonStart->setEnabled(false);
        return false;
    }
    else
    {
        foreach (const QString &s, slt) {
            ui->comboBoxComPort->addItem(s);
        }
        return true;
    }
    return true;
}

int MainWindow::detectNet()
{
    int cnt = 0;
    /*获取IP地址*/
    QString name = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(name);
    foreach(QHostAddress address, info.addresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            ui->comboBoxIP->addItem(address.toString());
            cnt++;
        }
    }
    return cnt;
}

void MainWindow::closeThread(QThread * const &pThread)
{
    if(pThread != NULL)
    {
        pThread->quit();
        pThread->wait();
        pThread->deleteLater();
    }
}



void MainWindow::on_pushButtonNibpDisp_clicked()
{
//    if(m_setNibpMsgHwnd != Q_NULLPTR)
//        m_setNibpMsgHwnd((HWND)this->winId());
    if(m_getWindowVisible != Q_NULLPTR)
    {
        if(!m_getWindowVisible())
        {
            if(m_displayNibp != Q_NULLPTR)
                m_displayNibp();
        }
        else
        {
//            if(m_displayNibpExOnPos != Q_NULLPTR)
//            {
//                m_displayNibpExOnPos(this->x(), this->y(), this->width(), this->height());
//            }
        }
    }
}

void MainWindow::on_pushButtonEcgSet_clicked()
{



    m_pDialogEcgSet->exec();
}

void MainWindow::on_pushButtonIbpSet_clicked()
{


    m_pDialogIbpSet->exec();
}

void MainWindow::on_pushButtonRespSet_clicked()
{


    m_pDialogRespSet->exec();
}

void MainWindow::on_pushButtonWaveSet_clicked()
{

    m_pDialogWaveSet->exec();
}

void MainWindow::on_pushButtonParaSet_clicked()
{
    m_pDialogParaSet->exec();
}

void MainWindow::on_pushButtonSpo2Set_clicked()
{

    m_pDialogSpo2Set->exec();
}

void MainWindow::on_pushButtonCo2Set_clicked()
{

    m_pDialogCo2Set->exec();
}

void MainWindow::setUserType(int type)
{
    ui->labelUser->setText(UserType[type]);
}

void MainWindow::changeWaveLead(DialogWaveSet::LeadMode mode, quint32 param)
{
    switch(mode)
    {
        case DialogWaveSet::Default:
            {
                Q_UNUSED(param);
                if(m_LeadMode == mode)
                    return;
                m_LeadMode = mode;

                m_pWaveTimer->stop();

                ui->groupBoxWave_1->show();
                ui->groupBoxWave_2->show();
                ui->groupBoxWave_3->show();
                ui->groupBoxWave_4->show();
                ui->groupBoxWave_5->show();
                ui->groupBoxWave_6->show();
                ui->groupBoxWave_7->show();
                ui->groupBoxWave_8->show();
                ui->groupBoxWave_9->show();
                ui->groupBoxWave_10->show();
                ui->groupBoxWave_11->show();
                ui->groupBoxWave_12->show();
                ui->groupBoxWave_13->hide();

                ui->labelWave_1->setText("I");
                ui->labelWave_2->setText("II");
                ui->labelWave_3->setText("V1");
                ui->labelWave_4->setText("V2");
                ui->labelWave_5->setText("SPO2");
                ui->labelWave_6->setText("CO2");
                ui->labelWave_7->setText("IBP1");
                ui->labelWave_8->setText("IBP2");
                ui->labelWave_9->setText("IBP3");
                ui->labelWave_10->setText("IBP4");
                ui->labelWave_11->setText("RESP");
                ui->labelWave_12->setText("RESP_SPO2");
                ui->labelIbp1WaveID->show();
                ui->labelIbp2WaveID->show();
                ui->labelIbp3WaveID->show();
                ui->labelIbp4WaveID->show();

                m_pWave[LabelWave::I]->setWaveEnabled(true);
                m_pWave[LabelWave::II]->setWaveEnabled(true);
                m_pWave[LabelWave::III]->setWaveEnabled(false);
                m_pWave[LabelWave::AVR]->setWaveEnabled(false);
                m_pWave[LabelWave::AVL]->setWaveEnabled(false);
                m_pWave[LabelWave::AVF]->setWaveEnabled(false);
                m_pWave[LabelWave::V1]->setParent(ui->groupBoxWave_3);
                m_pWave[LabelWave::V2]->setParent(ui->groupBoxWave_4);
                m_pWave[LabelWave::V1]->setWaveEnabled(true);
                m_pWave[LabelWave::V2]->setWaveEnabled(true);

                m_pWave[LabelWave::V1]->move(5,5);
                m_pWave[LabelWave::V2]->move(5,5);
                m_pWave[LabelWave::V1]->lower();
                m_pWave[LabelWave::V2]->lower();
                m_pWave[LabelWave::V3]->setWaveEnabled(false);
                m_pWave[LabelWave::V4]->setWaveEnabled(false);
                m_pWave[LabelWave::V5]->setWaveEnabled(false);
                m_pWave[LabelWave::V6]->setWaveEnabled(false);
                m_pWave[LabelWave::Spo2]->setWaveEnabled(true);
                m_pWave[LabelWave::Co2]->setWaveEnabled(true);
                m_pWave[LabelWave::Ibp1]->setWaveEnabled(true);
                m_pWave[LabelWave::Ibp2]->setWaveEnabled(true);
                m_pWave[LabelWave::Ibp3]->setWaveEnabled(true);
                m_pWave[LabelWave::Ibp4]->setWaveEnabled(true);
                m_pWave[LabelWave::Resp]->setWaveEnabled(true);
                m_pWave[LabelWave::Resp_s]->setWaveEnabled(true);
                for(int i = 0; i < LabelWave::MAX_Wave; i++)
                    m_pWave[i]->reset();
                LabelEcgWave::setReadCnt(4);
                m_pWaveTimer->start(DRAW_TIMER_CNT);

            }
            break;
        case DialogWaveSet::Ecg12Lead:
            {
                Q_UNUSED(param);
                if(m_LeadMode == mode)
                    return;
                m_LeadMode = mode;

                m_pWaveTimer->stop();

                ui->groupBoxWave_1->show();
                ui->groupBoxWave_2->show();
                ui->groupBoxWave_3->show();
                ui->groupBoxWave_4->show();
                ui->groupBoxWave_5->show();
                ui->groupBoxWave_6->show();
                ui->groupBoxWave_7->show();
                ui->groupBoxWave_8->show();
                ui->groupBoxWave_9->show();
                ui->groupBoxWave_10->show();
                ui->groupBoxWave_11->show();
                ui->groupBoxWave_12->show();
                ui->groupBoxWave_13->hide();

                ui->labelWave_1->setText("I");
                ui->labelWave_2->setText("II");
                ui->labelWave_3->setText("III");
                ui->labelWave_4->setText("AVR");
                ui->labelWave_5->setText("AVL");
                ui->labelWave_6->setText("AVF");
                ui->labelWave_7->setText("V1");
                ui->labelWave_8->setText("V2");
                ui->labelWave_9->setText("V3");
                ui->labelWave_10->setText("V4");
                ui->labelWave_11->setText("V5");
                ui->labelWave_12->setText("V6");
                ui->labelIbp1WaveID->hide();
                ui->labelIbp2WaveID->hide();
                ui->labelIbp3WaveID->hide();
                ui->labelIbp4WaveID->hide();

                m_pWave[LabelWave::I]->setWaveEnabled(true);
                m_pWave[LabelWave::II]->setWaveEnabled(true);
                m_pWave[LabelWave::III]->setWaveEnabled(true);
                m_pWave[LabelWave::AVR]->setWaveEnabled(true);
                m_pWave[LabelWave::AVL]->setWaveEnabled(true);
                m_pWave[LabelWave::AVF]->setWaveEnabled(true);
                m_pWave[LabelWave::V1]->setParent(ui->groupBoxWave_7);
                m_pWave[LabelWave::V2]->setParent(ui->groupBoxWave_8);
                m_pWave[LabelWave::V1]->setWaveEnabled(true);
                m_pWave[LabelWave::V2]->setWaveEnabled(true);

                m_pWave[LabelWave::V1]->move(5,5);
                m_pWave[LabelWave::V2]->move(5,5);
                m_pWave[LabelWave::V1]->lower();
                m_pWave[LabelWave::V2]->lower();
                m_pWave[LabelWave::V3]->setWaveEnabled(true);
                m_pWave[LabelWave::V4]->setWaveEnabled(true);
                m_pWave[LabelWave::V5]->setWaveEnabled(true);
                m_pWave[LabelWave::V6]->setWaveEnabled(true);
                m_pWave[LabelWave::Spo2]->setWaveEnabled(false);
                m_pWave[LabelWave::Co2]->setWaveEnabled(false);
                m_pWave[LabelWave::Ibp1]->setWaveEnabled(false);
                m_pWave[LabelWave::Ibp2]->setWaveEnabled(false);
                m_pWave[LabelWave::Ibp3]->setWaveEnabled(false);
                m_pWave[LabelWave::Ibp4]->setWaveEnabled(false);
                m_pWave[LabelWave::Resp]->setWaveEnabled(false);
                m_pWave[LabelWave::Resp_s]->setWaveEnabled(false);
                for(int i = 0; i < LabelWave::MAX_Wave; i++)
                    m_pWave[i]->reset();
                LabelEcgWave::setReadCnt(12);
                m_pWaveTimer->start(DRAW_TIMER_CNT);
            }
            break;
        case DialogWaveSet::Custom:
            {
                m_LeadMode = mode;

                m_pWaveTimer->stop();

                ui->groupBoxWave_1->hide();
                ui->groupBoxWave_2->hide();
                ui->groupBoxWave_3->hide();
                ui->groupBoxWave_4->hide();
                ui->groupBoxWave_5->hide();
                ui->groupBoxWave_6->hide();
                ui->groupBoxWave_7->hide();
                ui->groupBoxWave_8->hide();
                ui->groupBoxWave_9->hide();
                ui->groupBoxWave_10->hide();
                ui->groupBoxWave_11->hide();
                ui->groupBoxWave_12->hide();

                ui->groupBoxWave_13->move(0, 0);
                ui->groupBoxWave_13->resize(1280, 960);


                for(int i = 0; i < LabelWave::MAX_Wave; i++)
                {
                    if((int)param == i)
                    {
                        if(i >= LabelWave::Ibp_Start && i <= LabelWave::Ibp_End)
                        {
                            switch(i)
                            {
                                case LabelWave::Ibp1: ui->labelIbpWaveID->setText(ui->labelIbp1WaveID->text()); break;
                                case LabelWave::Ibp2: ui->labelIbpWaveID->setText(ui->labelIbp2WaveID->text()); break;
                                case LabelWave::Ibp3: ui->labelIbpWaveID->setText(ui->labelIbp3WaveID->text()); break;
                                case LabelWave::Ibp4: ui->labelIbpWaveID->setText(ui->labelIbp4WaveID->text()); break;
                            }
                            ui->labelIbpWaveID->move(70, 10);
                            ui->labelIbpWaveID->show();
                        }
                        else
                        {
                            ui->labelIbpWaveID->hide();
                        }
                        ui->labelWave_13->setText(LabelWaveDisc[i]);
                        ui->labelWave_13->move(10, 10);
                        ui->labelWave_13->show();
                        m_pWave[i]->setParent(ui->groupBoxWave_13);
                        m_pWave[i]->reload(1280 - 10, 960 - 10);
                        m_pWave[i]->setEnabled(true);
                        m_pWave[i]->move(5,5);
                        m_pWave[i]->lower();
                    }
                    else
                    {
                        m_pWave[i]->setEnabled(false);
                    }
                }
                ui->groupBoxWave_13->show();
                m_pWaveTimer->start(DRAW_TIMER_CNT);
            }
            break;
    }
}

void MainWindow::changeMainConfig(LabelID id, int param1, int param2)
{
    QSettings *configIni = new QSettings("params.ini", QSettings::IniFormat);

    switch(id)
    {
        case LABEL_ECG:
            {
                if(param1 == SetGain)
                {
                    configIni->setValue("Gain/Ecg", param2);
                    ui->labelEcgGain->setText(EcgGain[param2]);
                }
                else if(param1 == SetSpeed)
                {
                    configIni->setValue("WaveSpeed/Ecg", param2);
                }
            }
            break;
        case LABEL_SPO2:
            {
                if(param1 == SetSpeed)
                {
                    configIni->setValue("WaveSpeed/Spo2", param2);
                }
            }
            break;
        case LABEL_CO2:
            {
                if(param1 == SetSpeed)
                {
                    configIni->setValue("WaveSpeed/Co2", param2);
                }
            }
            break;
        case LABEL_IBP_START:
            {
                if(param1 == SetSpeed)
                {
                    configIni->setValue("WaveSpeed/Ibp", param2);
                }
            }
            break;
        case LABEL_RESP:
            {
                if(param1 == SetSpeed)
                {
                    configIni->setValue("WaveSpeed/Resp", param2);
                }
            }
            break;
        case LABEL_RESP_S:
            {
                if(param1 == SetSpeed)
                {
                    configIni->setValue("WaveSpeed/RespS", param2);
                }
            }
            break;
        default:
            {

            }
            break;
    }

    delete configIni;
}

void MainWindow::on_pushButtonNibpSend_clicked()
{
    if(m_pMyServer == Q_NULLPTR)
    {
        qDebug() << "Tcp Server is null";
        return;
    }
    if(m_iClientId == INVALID_DATA)
    {
        qDebug() << "Client is not be connected!";
        return;
    }

    NibpCommandField d;

    switch(ui->comboBoxNibpCommand->currentIndex())
    {
        case 0:
            d.command = StartNibp;
            if(m_setNibpStart != Q_NULLPTR)
                m_setNibpStart(true);
            break;
        case 1:
            d.command = StopNibp;
            if(m_setNibpStart != Q_NULLPTR)
                m_setNibpStart(false);
            break;
        case 2:
            d.command = FirstOverTest;
            if(m_setNibpStart != Q_NULLPTR)
                m_setNibpStart(true);
            break;
        case 3:
            d.command = SecondOverTest;
            if(m_setNibpStart != Q_NULLPTR)
                m_setNibpStart(true);
            break;
        case 4:
            d.command = StaticPress;
            if(m_setNibpStart != Q_NULLPTR)
                m_setNibpStart(true);
            break;
        case 5:
            d.command = AirLeak;
            if(m_setNibpStart != Q_NULLPTR)
                m_setNibpStart(true);
            break;
        case 6:
            d.command = CalPress;
            quint16 calPress = ui->lineEditCalPress->text().toShort();
            d.param1 = calPress & 0x00ff;
            d.param2 = (calPress & 0xff00) >> 8;
            if(m_setNibpStart != Q_NULLPTR)
                m_setNibpStart(true);
            break;

    }

    auto command = new CmsNibpCommand(CMS_ModuleNibp, CMS_LabelNibp);

    command->setCommand(d);

    QSharedPointer<ICmsData> data(command);

    emit sendCommand(m_iClientId, data);

    data.clear();


}

void MainWindow::on_pushButtonFreeze_clicked()
{
    if(ui->pushButtonFreeze->text() == "Freeze\nWave")
    {
        for(int i = 0; i < LabelWave::MAX_Wave; i++)
        {
            m_pWave[i]->freezeWave();
        }
        ui->pushButtonFreeze->setText("Unfreeze\nWave");
    }
    else
    {
        ui->pushButtonFreeze->setText("Freeze\nWave");
        for(int i = 0; i < LabelWave::MAX_Wave; i++)
        {
            m_pWave[i]->unfreezeWave();
        }
    }
}
