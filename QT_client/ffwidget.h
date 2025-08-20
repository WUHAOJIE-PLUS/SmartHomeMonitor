#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include "ffmpegkits.h"

class VideoPlayer : public QWidget {
    Q_OBJECT
public:
    explicit VideoPlayer(QWidget *parent = nullptr);
    ~VideoPlayer();
    void startPlayback(int);
    void stopPlayback();
    void pausePlayback();
    void setVoice(int value);

private:
    void setupUI();

    void connectSignals();

protected:
    void resizeEvent(QResizeEvent *event) override;
private:
    // UI组件
    QLabel *videoLabel;
    QSlider *volumeSlider;

    // FFmpeg组件
    FFmpegKits *ffmpeg;
};

