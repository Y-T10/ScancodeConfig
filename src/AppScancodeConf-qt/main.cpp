#include <QProgressBar>
#include <QApplication>
#include <QPushButton>
#include <QPointer>
#include <QWidget>
#include <QSlider>

class Window : public QWidget {
    public:
        explicit Window(QWidget* parent = nullptr) noexcept
        :QWidget(parent),
        m_button(new QPushButton("Hello World", this)),
        m_progress(new QProgressBar(this)),
        m_slider(new QSlider(this)){
            setFixedSize(800, 600);
            m_button->setGeometry(10, 10, 80, 30);

            connect(m_button, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));

            m_progress->setRange(0, 100);
            m_progress->setValue(0);
            m_progress->setGeometry(10, 60, 200, 20);

            m_slider->setOrientation(Qt::Horizontal);
            m_slider->setRange(0, 100);
            m_slider->setValue(0);
            m_slider->setGeometry(10, 100, 180, 30);

            QObject::connect(m_slider, SIGNAL (valueChanged(int)), m_progress, SLOT (setValue(int)));
        }
    
    private:
        QPointer<QPushButton> m_button;
        QPointer<QProgressBar> m_progress;
        QPointer<QSlider> m_slider;
};

int main(int argc, char* argv[]) {
    QPointer<QApplication> app (new QApplication(argc, argv));
    Window window;
    window.show();
    return app->exec();
}
