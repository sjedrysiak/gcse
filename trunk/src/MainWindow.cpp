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
#include "Params.h"

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
//	connect(&gcs, SIGNAL(terminated()), this, SLOT(gcsFinished()));
	connect(btnInitGrammar, SIGNAL(clicked()), this, SLOT(initGrammar()));
	connect(btnRun, SIGNAL(clicked()), this, SLOT(runGCS()));
	connect(btnLoadSentences, SIGNAL(clicked()), this, SLOT(readSentences()));
	connect(sbxEvolutionSteps, SIGNAL(valueChanged(int)), this, SLOT(changeMaxEvolutionSteps(int)));
	connect(btnAddSymbol, SIGNAL(clicked()), this, SLOT(addNonterminal()));
	connect(btnAddRule, SIGNAL(clicked()), this, SLOT(addRule()));
	connect(btnClearGrammar, SIGNAL(clicked()), this, SLOT(clearGrammar()));
}

void MainWindow::runGCS()
{
	action_Run->setEnabled(false);
	btnRun->setEnabled(false);
	gcs = new GCS(mGrammar, mSentences);
	connect(gcs, SIGNAL(finished()), this, SLOT(gcsFinished()));
	gcs->start();
}

void MainWindow::gcsFinished()
{
	action_Run->setEnabled(true);
	btnRun->setEnabled(true);
	delete gcs;
	gcs = NULL;
}

void MainWindow::showSettingsDialog()
{
	this->mSettingsDialog->show();
}

void MainWindow::initGrammar()
{
	this->mGrammar.initGrammar(this->sbxNonterminals->value(), this->sbxRules->value());
	this->listNonterminals->clear();
	for (int i = 0; i < this->mGrammar.NSet().size(); i++)
	{
		this->listNonterminals->addItem(this->mGrammar.NSet()[i]);
	}
	this->reloadCombos();
	this->listRules->clear();
	for (int i = 0; i < this->mGrammar.PNSet().size(); i++)
	{
		this->listRules->addItem(this->mGrammar.PNSet()[i]);
	}
}

void MainWindow::readSentences()
{
//	qDebug() << QString() + __FUNCTION__ + " start";
	this->mSentences.clear();
	QFile file(edtFilePath->text());
	if (file.open(QFile::ReadOnly))
	{
		QTextStream lines(&file);
		while (!lines.atEnd())
		{
			QString tmp = lines.readLine();
			if (tmp.size() > 4)
			{
				QTextStream line(&tmp);
				Sentence snt;
				int positive;
				line >> positive;
				line.skipWhiteSpace();
				int trash;
				line >> trash;//only for read second number from line (number useless at this moment)
				line.skipWhiteSpace();
				snt = line.readAll();
				if (positive == 0)
				{
					snt.setPositive(false);
				}
				else
				{
					snt.setPositive(true);
				}
				this->mSentences << snt;
			}
		}
		file.close();
	}
	this->listSentences->clear();
	for (int i = 0; i < mSentences.size(); i++)
	{
		this->listSentences->addItem(mSentences[i]);
	}
	//TODO obsługa błędów z plikiem
//	qDebug() << QString() + __FUNCTION__ + " end";
}

void MainWindow::changeMaxEvolutionSteps(int value)
{
	Params::setMaxEvolutionSteps(value);
}

void MainWindow::addNonterminal()
{
	if (!this->edtSymbol->text().isEmpty())
	{
		NSymbol symbol(this->edtSymbol->text());
		if (this->mGrammar.addSymbol(symbol))
		{
			this->listNonterminals->addItem(symbol);
			this->reloadCombos();
		}
	}
}

void MainWindow::addRule()
{
	QString s1, s2, s3;
	s1 = this->cbxAction1->currentText();
	s2 = this->cbxAction2->currentText();
	s3 = this->cbxCondition->currentText();
	if (!s1.isEmpty() && !s2.isEmpty() && !s3.isEmpty())
	{
		NCondition cond(s1, s2);
		Action act(s3);
		NClassifier cl(cond, act);
		if (this->mGrammar.addClNormal(cl))
		{
			this->listRules->addItem(cl);
		}
	}
}

void MainWindow::reloadCombos()
{
	this->cbxCondition->clear();
	this->cbxAction1->clear();
	this->cbxAction2->clear();
	for (int i = 0; i < this->mGrammar.NSet().size(); i++)
	{
		this->cbxCondition->addItem(this->mGrammar.NSet()[i], this->mGrammar.NSet()[i]);
		this->cbxAction1->addItem(this->mGrammar.NSet()[i], this->mGrammar.NSet()[i]);
		this->cbxAction2->addItem(this->mGrammar.NSet()[i], this->mGrammar.NSet()[i]);
	}
}

void MainWindow::clearGrammar()
{
	this->mGrammar.initGrammar();
	this->listNonterminals->clear();
	this->listRules->clear();
	this->reloadCombos();
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
	if (gcs != NULL && gcs->isRunning())
	{
		gcs->terminate();
		gcs->wait();
		delete gcs;
		gcs = NULL;
	}
}
