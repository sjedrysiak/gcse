/****************************************************************************
 **
 ** Trolltech hereby grants a license to use the Qt/Eclipse Integration
 ** plug-in (the software contained herein), in binary form, solely for the
 ** purpose of creating code to be used with Trolltech's Qt software.
 **
 ** Qt Designer is licensed under the terms of the GNU General Public
 ** License versions 2.0 and 3.0 ("GPL License"). Trolltech offers users the
 ** right to use certain no GPL licensed software under the terms of its GPL
 ** Exception version 1.2 (http://trolltech.com/products/qt/gplexception).
 **
 ** THIS SOFTWARE IS PROVIDED BY TROLLTECH AND ITS CONTRIBUTORS (IF ANY) "AS
 ** IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 ** TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 ** PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 ** OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 ** EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 ** PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 ** PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 ** LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 ** NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 ** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ** Since we now have the GPL exception I think that the "special exception
 ** is no longer needed. The license text proposed above (other than the
 ** special exception portion of it) is the BSD license and we have added
 ** the BSD license as a permissible license under the exception.
 **
 ****************************************************************************/

#include "MainWindow.h"
#include <QMessageBox>

#include <QtCore>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	this->setupUi(this);
	this->setupActions();
	this->mSettingsDialog = new SettingsDialog(this);
}

void MainWindow::setupActions()
{
	connect(action_Close, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
	connect(action_About, SIGNAL(triggered(bool)), this, SLOT(about()));
	connect(action_Run, SIGNAL(triggered(bool)), this, SLOT(runGCS()));
	connect(action_Settings, SIGNAL(triggered(bool)), this, SLOT(showSettingsDialog()));
	connect(&gcs, SIGNAL(finished()), this, SLOT(gcsFinished()));
	connect(&gcs, SIGNAL(terminated()), this, SLOT(gcsFinished()));
}

void MainWindow::runGCS()
{
	action_Run->setEnabled(false);
	gcs.start();
}

void MainWindow::gcsFinished()
{
	action_Run->setEnabled(true);
}

void MainWindow::showSettingsDialog()
{
	this->mSettingsDialog->show();
}

void MainWindow::about()
{
	QString system;
#ifdef Q_WS_X11
	system = "Linux";
#endif

#ifdef Q_WS_WIN
	system = "Windows";
#endif
	QMessageBox::about(this, tr("About GCSE"), tr("GCSE 0.1 (Grammar-based Classifier System Ensemble)\n"
	                                              "Visit <<wstawić link do strony>>\n"
	                                              "Program working on platform %1.\n"
	                                              "© 2008 Sylwester Jędrysiak").arg(system));
}

MainWindow::~MainWindow()
{
	if (gcs.isRunning())
	{
		gcs.terminate();
	}
	gcs.wait();
}
