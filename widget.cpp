#include "widget.h"
#include "ui_widget.h"
#include <QMenu>
#include <QDialog>
#include <QVBoxLayout>
#include <QSlider>
#include <QDialogButtonBox>
#include <QTimer>
#include <QSound>
#include <QMediaPlayer>
#include <QDir>
#include <QFileInfo>
#include <vector>
#include <QString>
#include <QMediaMetaData>
#include <random>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 设置窗口属性，使背景透明和去除标准边框
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);

    // 创建阴影效果
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(5);    // 阴影的模糊半径
    shadowEffect->setXOffset(.5);       // 阴影在X方向上的偏移量
    shadowEffect->setYOffset(.5);       // 阴影在Y方向上的偏移量
    shadowEffect->setColor(Qt::gray);  // 阴影的颜色

    // 应用阴影效果到小部件
    ui->mainFrame->setGraphicsEffect(shadowEffect);

    setMouseTracking(true);
    //set play button to 'play.png'
    ui->playButton->setStyleSheet("background-image: url(':/icon/play.png');");
    //init countdown timer
    countdownTimer = new QTimer(this);
    connect(countdownTimer, &QTimer::timeout, this, &Widget::updateCountdown);
    //set media player
    player = new QMediaPlayer(this);
    player->setVolume(20);
    dj = new QMediaPlayer(this);
    connect(dj, &QMediaPlayer::mediaStatusChanged, this, &Widget::playNextSong);
    playRandomSong(dj);
    dj->pause();
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_crossButton_clicked()
{
    qDebug()<<"mamba out";
    this->close();
}

// drag and menu
void Widget::mousePressEvent(QMouseEvent *event) {

    if (event->button() == Qt::LeftButton) {
        player->setMedia(QUrl("qrc:/audio/border.wav"));
        player->play();
        // 如果按下的是左键，则记录鼠标按下位置与窗口当前位置之间的偏移
        offset = event->globalPos() - this->pos();

    }else if (event->button() == Qt::RightButton){
        player->setMedia(QUrl("qrc:/audio/clicked.wav"));
        player->play();
        // 如果按下的是右键，则弹出上下文菜单
        QMenu contextMenu(this);

        // 设置菜单样式
        contextMenu.setStyleSheet("QMenu {"
                                  "background-color: white;"  // 背景色为白色
                                  "border-radius: 10px;"      // 圆角边缘
                                  "font-family: 'Microsoft YaHei';"  // 设置字体为微软雅黑
                                  "}"
                                  "QMenu::item {"
                                  "padding: 8px 16px;"        // 设置菜单项的内边距
                                  "}"
                                  "QMenu::item:selected {"
                                  "background-color: rgb(187, 213, 196);" // 被选中的菜单项背景色
                                  "}");



        // 添加菜单项
        QAction *option1 = contextMenu.addAction("时长");
        QAction *option2 = contextMenu.addAction("音乐");
        // ... 可以继续添加其他选项 ...

        // 连接菜单项的信号与槽
        connect(option1, &QAction::triggered, this, &Widget::onOption1Clicked);
        connect(option2, &QAction::triggered, this, &Widget::onOption2Clicked);
        // ... 连接其他选项的信号与槽 ...
        connect(option1, &QAction::hovered, this, &Widget::playTick);
        connect(option2, &QAction::hovered, this, &Widget::playTick);
        // 在鼠标点击的位置弹出上下文菜单
        QPoint menuPos = event->globalPos();
        menuPos.setX(menuPos.x()+2);
        menuPos.setY(menuPos.y()+2);
        contextMenu.exec(menuPos);
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        // 如果左键被按住，计算新的窗口位置
        QPoint newpos = event->globalPos() - offset;
        this->move(newpos);
    }
}


void Widget::on_playButton_clicked()
{
    if(b_pbuttonon==true){
        ui->playButton->setStyleSheet("background-image: url(':/icon/stop.png');");
        b_pbuttonon = false;
        //start countdown
        startPomo(selectedTime);

    }else{
        ui->playButton->setStyleSheet("background-image: url(':/icon/play.png');");
        b_pbuttonon = true;
        player->setMedia(QUrl("qrc:/audio/clicked.wav"));
        player->play();
        //stop countdown
        countdownTimer->stop();
        ui->timeLabel->setText("00:00");
        dj->pause();

    }
}

// 选择时长
void Widget::onOption1Clicked() {
    // 处理选项1的点击事件
    player->setMedia(QUrl("qrc:/audio/clicked.wav"));
    player->play();
    // 创建一个模态对话框
        QDialog *timeDialog = new QDialog(this);
//        timeDialog->setWindowTitle("选择时间");

        // 移除标题栏
        timeDialog->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

        QVBoxLayout *layout = new QVBoxLayout(timeDialog);

        // 创建滑块
        QSlider *timeSlider = new QSlider(Qt::Horizontal);
        timeSlider->setMinimum(5);          // 最小值为5分钟
        timeSlider->setMaximum(45);         // 最大值为45分钟
        timeSlider->setTickPosition(QSlider::TicksBelow); // 设置滑块下面的刻度线
        timeSlider->setTickInterval(5);     // 每5分钟一个刻度

        layout->addWidget(timeSlider);

        QLabel *timeLabel = new QLabel(timeDialog);
        timeLabel->setText("5分钟");
        layout->addWidget(timeLabel);

        // 连接滑块的值改变信号与标签的槽，以显示当前选择的时间
        connect(timeSlider, &QSlider::valueChanged, [timeLabel](int value) {
            timeLabel->setText(QString::number(value) + "分钟");
        });

        // 添加确定按钮
        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, timeDialog);
        connect(buttonBox, &QDialogButtonBox::accepted, timeDialog, &QDialog::accept);
        layout->addWidget(buttonBox);

        // 设置样式
        timeDialog->setStyleSheet(
            "QDialog {"
            "background-color: white;"  // 背景色为白色
            "border-radius: 10px;"      // 圆角边缘
            "}"
            "QPushButton {"
            "background-color: rgb(187, 213, 196);" // 按钮背景色
            "border-radius: 5px;"                   // 按钮圆角
            "padding: 5px 15px;"                    // 按钮内边距
            "}"
            "QPushButton:hover {"
            "background-color: rgb(167, 193, 176);" // 按钮悬停时的背景色
            "}"
        );

        // 显示模态对话框
        if (timeDialog->exec() == QDialog::Accepted) {
            // 用户点击了确定按钮
            int selectedTime = timeSlider->value();
            // TODO: 处理选定的时间
            qDebug()<<selectedTime;
            this->selectedTime = selectedTime*60+1;//turn mins to secs
            //start countdown
            ui->playButton->setStyleSheet("background-image: url(':/icon/stop.png');");
            b_pbuttonon = false;
            startPomo(selectedTime*60+1);

        }

        // 清理资源
        delete timeDialog;
}

void Widget::onOption2Clicked() {
    // 创建音乐播放器对话框
    musicPlayerDialog = new QDialog(this);
    musicPlayerDialog->setStyleSheet(
                "QDialog {"
                "background-color: white;"  // 背景色为白色
                "border-radius: 10px;"      // 圆角边缘
                "}"
                "QPushButton {"
                "background-color: rgb(187, 213, 196);" // 按钮背景色
                "border-radius: 5px;"                   // 按钮圆角
                "padding: 5px 15px;"                    // 按钮内边距
                "}"
                "QPushButton:hover {"
                "background-color: rgb(167, 193, 176);" // 按钮悬停时的背景色
                "}"
            );
    musicPlayerDialog->setWindowTitle("Music Player");
    musicPlayerDialog->setMinimumWidth(300);

    QVBoxLayout *layout = new QVBoxLayout(musicPlayerDialog);

    // 歌曲标题标签
    songTitleLabel = new QLabel("Song Title");
    songTitleLabel->setText(dj->currentMedia().canonicalUrl().fileName());
    layout->addWidget(songTitleLabel);

    // 音乐播放器
    connect(dj, &QMediaPlayer::mediaChanged, [=](const QMediaContent &content) {
        songTitleLabel->setText(content.canonicalUrl().fileName());
    });

    // 音量滑块
    volumeSlider = new QSlider(Qt::Horizontal);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);
    layout->addWidget(volumeSlider);
    connect(volumeSlider, &QSlider::valueChanged, dj, &QMediaPlayer::setVolume);

    // 下一首歌曲按钮
    nextSongButton = new QPushButton("Next Song");
    layout->addWidget(nextSongButton);
    connect(nextSongButton, &QPushButton::clicked, [=]() {
        // TODO: 更改为下一首歌曲的逻辑
        playRandomSong(dj);
    });

    musicPlayerDialog->setLayout(layout);
    musicPlayerDialog->show();
}

//start this round of pomodoro
void Widget::startPomo(int duration) {

    player->setMedia(QUrl("qrc:/audio/start.wav"));
    player->play();

//    playRandomSong(dj);
    dj->play();
    startCountdown(duration);
}

void Widget::startCountdown(int duration){
    remainingTime = duration;  // 设置开始的倒计时时间，例如，5分钟为5*60 = 300秒
    countdownTimer->start(1000);  // 每秒更新一次
    updateCountdown();  // 立即更新显示
}

void Widget::updateCountdown() {
    // 减少剩余时间
    remainingTime--;

    // 计算分钟和秒
    int minutes = remainingTime / 60;
    int seconds = remainingTime % 60;

    // 更新标签显示
    ui->timeLabel->setText(QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));

    // 如果时间到了，停止计时器
    if (remainingTime <= 0) {
        countdownTimer->stop();
        dj->pause();
        ui->playButton->setStyleSheet("background-image: url(':/icon/play.png');");
        b_pbuttonon = true;
    }
}

void Widget::playTick(){
//    player->setMedia(QUrl("qrc:/audio/tick.wav"));
//    player->play();
//    qDebug()<<"hover";
}

std::vector<QString> getAllFilesInFolder(const QString &relativeFolderPath) {
    std::vector<QString> filePaths;

    QDir dir(relativeFolderPath);
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);

    for (const QFileInfo &fileInfo : fileInfoList) {
        if (fileInfo.isFile()) {
            filePaths.push_back(fileInfo.absoluteFilePath());
        }
    }

    return filePaths;
}

void playRandomSong(QMediaPlayer* dj){
    auto fileVector = getAllFilesInFolder("music");
    std::vector<qint64> durations;
    qint64 totalDuration = 0;

    // 获取每个文件的长度
    for (const QString &filePath : fileVector) {
        QMediaPlayer tempPlayer;
        tempPlayer.setMedia(QUrl::fromLocalFile(filePath));
        while (!tempPlayer.isMetaDataAvailable())
            QCoreApplication::processEvents();

        qint64 duration = tempPlayer.duration(); // in milliseconds
        durations.push_back(duration);
        if (duration > 600000) {  // over 10 minutes
            totalDuration += duration;  // weighted by their actual duration
        } else {
            totalDuration += 600000;   // treat as 10 minutes
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<qint64> dis(0, totalDuration-1);

    qint64 randomValue = dis(gen);
    QString selectedFile;
    qint64 accumulatedDuration = 0;

    for (size_t i = 0; i < fileVector.size(); i++) {
        qint64 effectiveDuration = durations[i] > 600000 ? durations[i] : 600000;
        if (randomValue < accumulatedDuration + effectiveDuration) {
            selectedFile = fileVector[i];
            break;
        }
        accumulatedDuration += effectiveDuration;
    }
    dj->setMedia(QUrl::fromLocalFile(selectedFile));

    // 如果音乐长度超过10分钟，从随机位置开始
    if (dj->duration() > 600000) {
        std::uniform_int_distribution<qint64> disPos(0, dj->duration() - 300000 - 1);  // don't start in the last 5 minutes
        dj->setPosition(disPos(gen));
    }

    dj->play();
}

void Widget::playNextSong(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::EndOfMedia) {
        // 获取新的随机歌曲并设置为媒体源
        playRandomSong(dj);
    }
}






