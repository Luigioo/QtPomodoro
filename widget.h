#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsDropShadowEffect>
#include <QtDebug>
#include <QMouseEvent>
#include <QMediaPlayer>
#include <QDialog>
#include <QLabel>
#include <QSlider>



QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void on_crossButton_clicked();

    void on_playButton_clicked();
    void onOption1Clicked();
    void onOption2Clicked();
    void startPomo(int duration);
    void updateCountdown();
    void playTick();
    void startCountdown(int duration);
    void playNextSong(QMediaPlayer::MediaStatus status);

private:
    Ui::Widget *ui;
    QPoint offset;
    bool b_pbuttonon = true;
    QTimer *countdownTimer;
    int remainingTime;  // 以秒为单位
    int selectedTime = 25*60+1;
    QMediaPlayer *player;
    QMediaPlayer *dj;
    QDialog *musicPlayerDialog;
    QLabel *songTitleLabel;
    QSlider *volumeSlider;
    QPushButton *nextSongButton;
};

std::vector<QString> getAllFilesInFolder(const QString &relativeFolderPath);
void playRandomSong(QMediaPlayer* dj);
#endif // WIDGET_H
