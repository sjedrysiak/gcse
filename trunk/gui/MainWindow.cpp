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
#include <QFileDialog>
#include <QTextStream>
#include "../src/Params.h"
#include <QMutexLocker>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	setupUi(this);
	setupActions();
	initValues();
	mSettingsDialog = new SettingsDialog(this);
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
	connect(sbxNonterminals, SIGNAL(valueChanged(int)), this, SLOT(changeNonterminalsAmount(int)));
	connect(sbxRules, SIGNAL(valueChanged(int)), this, SLOT(changeStartRulesAmount(int)));
	connect(sbxThreads, SIGNAL(valueChanged(int)), this, SLOT(changeThreadsAmount(int)));
	connect(sbxIterations, SIGNAL(valueChanged(int)), this, SLOT(changeIterationsAmount(int)));
	connect(sbxEvolutionSteps, SIGNAL(valueChanged(int)), this, SLOT(changeMaxEvolutionSteps(int)));
	connect(cbxEndOnFull, SIGNAL(toggled(bool)), this, SLOT(changeEndOnFullFintess(bool)));
	connect(btnAddSymbol, SIGNAL(clicked()), this, SLOT(addNonterminal()));
	connect(btnAddRule, SIGNAL(clicked()), this, SLOT(addRule()));
	connect(btnClearGrammar, SIGNAL(clicked()), this, SLOT(clearGrammar()));
	connect(btnBrowse, SIGNAL(clicked()), this, SLOT(sentencesFileBrowse()));
}

void MainWindow::runGCS()
{
	action_Run->setEnabled(false);
	btnRun->setEnabled(false);
	GCS* gcs;
	for (int i = 0; i < Params::instance().threads; i++)
	{
		gcs = new GCS(mGrammar, mSentences, *this);
		connect(gcs, SIGNAL(finished()), this, SLOT(gcsFinished()));
		gcs->start();
//		qDebug() << "gcs" << i << "started" << gcs;
		gcsList << gcs;
	}
}

void MainWindow::gcsFinished()
{
//	qDebug() << "gcs finished...";
	QMutexLocker locker(&mutex);
	if (gcsList.size() == 1)
	{
		delete gcsList[0];
		gcsList.clear();
		action_Run->setEnabled(true);
		btnRun->setEnabled(true);
	}
	else
	{
		for (int i = 0, size = gcsList.size(); i < size; i++)
		{
			if (gcsList[i]->isFinished())
			{
				delete gcsList[i];
				gcsList.removeAt(i);
				break;
			}
		}
	}
}

void MainWindow::showSettingsDialog()
{
	mSettingsDialog->show();
}

void MainWindow::initGrammar()
{
	mGrammar.initGrammar(sbxNonterminals->value(), sbxRules->value());
	listNonterminals->clear();
	for (int i = 0, size = mGrammar.N.size(); i < size; i++)
	{
		listNonterminals->addItem(mGrammar.N[i]);
	}
	reloadCombos();
	listRules->clear();
	for (int i = 0, size = mGrammar.PN.size(); i < size; i++)
	{
		listRules->addItem(mGrammar.PN[i].toString());
	}
}

void MainWindow::readSentences()
{
//	qDebug() << QString() + __FUNCTION__ + " start";
	mSentences.clear();
	QFile file(edtFilePath->text());
	if (file.open(QFile::ReadOnly))
	{
		QTextStream lines(&file);
		if (!lines.atEnd())
		{
			lines.readLine();//first line is useless
		}
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
					snt.isPositive = false;
				}
				else
				{
					snt.isPositive = true;
				}
				mSentences << snt;
			}
		}
		file.close();
	}
	listSentences->clear();
	for (int i = 0, size = mSentences.size(); i < size; i++)
	{
		listSentences->addItem(mSentences[i].toString());
	}
	//TODO obsługa błędów z plikiem
//	qDebug() << QString() + __FUNCTION__ + " end";
}

void MainWindow::changeNonterminalsAmount(int value)
{
	Params::instance().nonterminalSymbolsAmount = value;
}

void MainWindow::changeStartRulesAmount(int value)
{
	Params::instance().startNonterminalRules = value;
}

void MainWindow::changeThreadsAmount(int value)
{
	Params::instance().threads = value;
}

void MainWindow::changeIterationsAmount(int value)
{
	Params::instance().iterations = value;
}

void MainWindow::changeMaxEvolutionSteps(int value)
{
	Params::instance().maxEvolutionSteps = value;
}

void MainWindow::changeEndOnFullFintess(bool value)
{
	Params::instance().endOnFullFitness = value;
}

void MainWindow::addNonterminal()
{
	if (!edtSymbol->text().isEmpty())
	{
		NSymbol symbol(edtSymbol->text());
		if (mGrammar.addSymbol(symbol))
		{
			listNonterminals->addItem(symbol);
			reloadCombos();
		}
	}
}

void MainWindow::addRule()
{
	QString s1, s2, s3;
	s1 = cbxAction1->currentText();
	s2 = cbxAction2->currentText();
	s3 = cbxCondition->currentText();
	if (!s1.isEmpty() && !s2.isEmpty() && !s3.isEmpty())
	{
		NCondition cond(s1, s2);
		Action act(s3);
		NClassifier cl(cond, act);
		if (mGrammar.addClNormal(cl))
		{
			listRules->addItem(cl.toString());
		}
	}
}

void MainWindow::reloadCombos()
{
	cbxCondition->clear();
	cbxAction1->clear();
	cbxAction2->clear();
	for (int i = 0, size = mGrammar.N.size(); i < size; i++)
	{
		cbxCondition->addItem(mGrammar.N[i], mGrammar.N[i]);
		cbxAction1->addItem(mGrammar.N[i], mGrammar.N[i]);
		cbxAction2->addItem(mGrammar.N[i], mGrammar.N[i]);
	}
}

void MainWindow::sentencesFileBrowse()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Open file with sentences"), ".", tr("Text files (*.txt);;All files (*)"));
	edtFilePath->setText(file);
}

void MainWindow::clearGrammar()
{
	mGrammar.initGrammar();
	listNonterminals->clear();
	listRules->clear();
	reloadCombos();
}

void MainWindow::initValues()
{
	Params& p = Params::instance();
	sbxNonterminals->setValue(p.nonterminalSymbolsAmount);
	sbxRules->setValue(p.startNonterminalRules);
	sbxThreads->setValue(p.threads);
	sbxIterations->setValue(p.iterations);
	sbxEvolutionSteps->setValue(p.maxEvolutionSteps);
	cbxEndOnFull->setChecked(p.endOnFullFitness);
}

void MainWindow::sendRules(QList<NClassifier> list)
{
	QMutexLocker locker(&mutex);
	int size = gcsList.size();
	if (size > 1)
	{
		GCS* dest = gcsList[Random::rand(size)];
		dest->sendRules(list);
	}
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
//	if (gcs != NULL && gcs->isRunning())
//	{
//		gcs->terminate();
//		gcs->wait();
//		delete gcs;
//		gcs = NULL;
//	}
}
