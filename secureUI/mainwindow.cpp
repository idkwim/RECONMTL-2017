#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "setupdialog.h"
#include "epgdialog.h"
#include "sgxware.h"
#include "encryptedfile.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);
    vw = new QVideoWidget(this);
    player->setVideoOutput(vw);
    this->setCentralWidget(vw);

    slider = new QSlider(this);
    bar = new QProgressBar(this);

    slider->setOrientation(Qt::Horizontal);

    ui->statusBar->addPermanentWidget(slider);
    ui->statusBar->addPermanentWidget(bar);

    connect(player,&QMediaPlayer::durationChanged,slider,&QSlider::setMaximum);
    connect(player,&QMediaPlayer::positionChanged,slider,&QSlider::setValue);
    connect(slider,&QSlider::sliderMoved,player,&QMediaPlayer::setPosition);

    connect(player,&QMediaPlayer::durationChanged,bar,&QProgressBar::setMaximum);
    connect(player,&QMediaPlayer::positionChanged,bar,&QProgressBar::setValue);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
/*	static EncryptedFile * s_encFile = nullptr;
    QString filename = QFileDialog::getOpenFileName(this,"Open a File","","Video File (*.*)");
    on_actionStop_triggered();

	on_actionStop_triggered();
	if (s_encFile) delete s_encFile;
	s_encFile = new EncryptedFile(0);
	s_encFile->setFileName(filename.toLatin1().data());
	s_encFile->open(QFile::ReadOnly);
	QUrl locUrl = QUrl::fromLocalFile(filename);
	player->setMedia(locUrl  , s_encFile);

	on_actionPlay_triggered();

	// from nowe on using open button for simple enclaves tests
	
	// test 0 - unaligned read from the encrypted file
	unsigned char chunk[1024 * 30];
	// pass
	// test 1 - download and read the tail of the movie
	bool bres = QFile::remove(QString("C:/Users/atlas/Documents/lib_debug/movie.7"));
	SGXware::getInstance()->prepareMovie(7);
	size_t real_movie_size = 29904006;

	SGXware::getInstance()->readMovieChunk(0, 1024, chunk);
	SGXware::getInstance()->readMovieChunk(1024, 1024, chunk);
	SGXware::getInstance()->readMovieChunk(1024 * 2, 1024, chunk);
	SGXware::getInstance()->readMovieChunk(1024 * 3, 1024, chunk);
	SGXware::getInstance()->readMovieChunk(1024 * 3 + 1, 1024, chunk);
	SGXware::getInstance()->readMovieChunk(1, 1027, chunk);
	SGXware::getInstance()->readMovieChunk(312, 2632, chunk);
	SGXware::getInstance()->readMovieChunk(real_movie_size - 1027, 1024, chunk);

*/
	on_actionManageEPG_triggered();

}

void MainWindow::on_actionPlay_triggered()
{
    player->play();
    ui->statusBar->showMessage("Playing");
}

void MainWindow::on_actionPause_triggered()
{
    player->pause();
    ui->statusBar->showMessage("Paused...");
}

void MainWindow::on_actionStop_triggered()
{
    player->stop();
    ui->statusBar->showMessage("Stopped");
}

void MainWindow::on_actionManageEPG_triggered()
{
    EPGDialog *pdlg = new EPGDialog;
	char movie_file_name[1024];
    unsigned int movie_to_play = (unsigned int) -1;
	static EncryptedFile * s_encFile = nullptr;

    pdlg->exec();
    movie_to_play = pdlg->selected_movie;
    delete pdlg;
    SGXware *pSGX = SGXware::getInstance();
    if (movie_to_play != (unsigned int)(-1) && pSGX->getFileName(movie_to_play, 1024, movie_file_name) )
    {
        on_actionStop_triggered();
		if (s_encFile) delete s_encFile;
		s_encFile = new EncryptedFile(movie_to_play);
		s_encFile->open(QFile::ReadOnly);
        player->setMedia(QUrl::fromLocalFile(movie_file_name), s_encFile); 

		on_actionPlay_triggered();

    }
}

void MainWindow::on_actionClean_the_library_triggered()
{

}

void MainWindow::on_actionSetup_triggered()
{
    SetupDialog *pdlg = new SetupDialog;
    pdlg->exec();
	pdlg->finalize();
    delete pdlg;

}
