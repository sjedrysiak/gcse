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
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	setupUi(this);
	setupActions();
	initValues();
	mSettingsDialog = new SettingsDialog(this);
	barsLayout = new QVBoxLayout(progressBarsArea);
	progressBarsArea->setLayout(barsLayout);
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
	connect(btnLoadSentences, SIGNAL(clicked()), this, SLOT(loadLearningSet()));
	connect(btnLoadTest, SIGNAL(clicked()), this, SLOT(loadTestingSet()));
	connect(sbxNonterminals, SIGNAL(valueChanged(int)), this, SLOT(changeNonterminalsAmount(int)));
	connect(sbxRules, SIGNAL(valueChanged(int)), this, SLOT(changeStartRulesAmount(int)));
	connect(sbxThreads, SIGNAL(valueChanged(int)), this, SLOT(changeThreadsAmount(int)));
	connect(sbxIterations, SIGNAL(valueChanged(int)), this, SLOT(changeIterationsAmount(int)));
	connect(sbxEvolutionSteps, SIGNAL(valueChanged(int)), this, SLOT(changeMaxEvolutionSteps(int)));
	connect(cbxEndOnFull, SIGNAL(toggled(bool)), this, SLOT(changeEndOnFullFintess(bool)));
	connect(btnAddSymbol, SIGNAL(clicked()), this, SLOT(addNonterminal()));
	connect(btnAddRule, SIGNAL(clicked()), this, SLOT(addRule()));
	connect(btnClearGrammar, SIGNAL(clicked()), this, SLOT(clearGrammar()));
	connect(btnBrowse, SIGNAL(clicked()), this, SLOT(learningSetFileBrowse()));
	connect(btnBrowseTest, SIGNAL(clicked()), this, SLOT(testingSetFileBrowse()));
}

void MainWindow::runGCS()
{
	action_Run->setEnabled(false);
	groupBox_2->setEnabled(false);
	Params& p = Params::instance();
	QList<QList<Sentence> > learningSets;

	if (p.splitLearningSet)
	{
		for (int i = 0; i < p.threads; i++)
		{
			learningSets << QList<Sentence> ();
		}
		for (int i = 0, size = mLearningSetPositive.size(); i < size; i++)
		{
			learningSets[i % p.threads] << mLearningSetPositive[i];
		}
		for (int i = 0, size = mLearningSetNegative.size(); i < size; i++)
		{
			learningSets[i % p.threads] << mLearningSetNegative[i];
		}
	}
	GCS* gcs;
	for (int i = 0; i < p.threads; i++)
	{
		if (p.splitLearningSet)
		{
			gcs = new GCS(mGrammar, learningSets[i], *this);
			prepareBars(gcs, learningSets[i].size());
		}
		else
		{
			gcs = new GCS(mGrammar, mLearningSetAll, *this);
			prepareBars(gcs, mLearningSetAll.size());
		}
		connect(gcs, SIGNAL(finished()), this, SLOT(gcsFinished()));
		gcs->start();
//		qDebug() << "gcs" << i << "started" << gcs;
		gcsList << gcs;
	}
}

void MainWindow::prepareBars(GCS* thread, int sentences)
{
	barsLayout->addWidget(new QLabel("Thread " + QString::number(thread->threadNumber) + ":"));
//	if (sentences > 100)
//	{
//		QProgressBar* sentBar = new QProgressBar(progressBarsArea);
//		sentBar->setMaximum(sentences);
//		sentBar->setValue(0);
//		sentBar->setAlignment(Qt::AlignHCenter);
//		sentBar->setFormat("Sentences parsed: %v/%m");
//		connect(thread, SIGNAL(parsedSentenceChanged(int)), sentBar, SLOT(setValue(int)));
//		barsLayout->addWidget(sentBar);
//	}

	QProgressBar* stepsBar = new QProgressBar(progressBarsArea);
	stepsBar->setMaximum(sbxEvolutionSteps->value());
	stepsBar->setValue(0);
	stepsBar->setAlignment(Qt::AlignHCenter);
	stepsBar->setFormat("Steps done: %v/%m");
	connect(thread, SIGNAL(stepChanged(int)), stepsBar, SLOT(setValue(int)));
	barsLayout->addWidget(stepsBar);

	QProgressBar* itersBar = new QProgressBar(progressBarsArea);
	itersBar->setMaximum(sbxIterations->value());
	itersBar->setValue(0);
	itersBar->setAlignment(Qt::AlignHCenter);
	itersBar->setFormat("Iterations done: %v/%m");
	connect(thread, SIGNAL(iterChanged(int)), itersBar, SLOT(setValue(int)));
	barsLayout->addWidget(itersBar);
}

void MainWindow::runTest()
{
	Params::instance().learningMode = false;
	gcsList << new GCS(mGrammarForTest, mTestingSet, *this);
	connect(gcsList[0], SIGNAL(finished()), this, SLOT(testFinished()));

	QProgressBar* sentBar = new QProgressBar(progressBarsArea);
	sentBar->setMaximum(mTestingSet.size());
	sentBar->setValue(0);
	sentBar->setAlignment(Qt::AlignHCenter);
	sentBar->setFormat("Sentences parsed: %v/%m");
	connect(gcsList[0], SIGNAL(parsedSentenceChanged(int)), sentBar, SLOT(setValue(int)));
	barsLayout->addWidget(new QLabel("Test progress:"));
	barsLayout->addWidget(sentBar);

	gcsList[0]->start();
//	qDebug() << "test started";
}

void MainWindow::testFinished()
{
	delete gcsList[0];
	gcsList.clear();
	Params::instance().learningMode = true;
	action_Run->setEnabled(true);
	groupBox_2->setEnabled(true);
}

void MainWindow::gcsFinished()
{
//	qDebug() << "gcs finished...";
	QMutexLocker locker(&mutex);
	Params& p = Params::instance();
	if (gcsList.size() == 1)
	{
		//		mGrammarForTest = gcsList[0]->getBestGrammar();
		mOutGrammars << gcsList[0]->getBestGrammar();
		delete gcsList[0];
		gcsList.clear();
		if (p.splitLearningSet)
		{
			mergeGrammars();
		}
		else
		{
			mGrammarForTest = mOutGrammars[0];
		}
		if (cbxRunTest->isChecked())
		{
			runTest();
		}
		else
		{
			action_Run->setEnabled(true);
			groupBox_2->setEnabled(true);
		}
	}
	else
	{
		for (int i = 0, size = gcsList.size(); i < size; i++)
		{
			if (gcsList[i]->isFinished())
			{
				mOutGrammars << gcsList[i]->getBestGrammar();
				delete gcsList[i];
				gcsList.removeAt(i);
				break;
			}
		}
	}
}

void MainWindow::mergeGrammars()
{
//	qDebug() << "merging started";
	if (mOutGrammars.isEmpty())
	{
		return;
	}
	mGrammarForTest = mOutGrammars[0];
	for (int i = 1, size = mOutGrammars.size(); i < size; i++)
	{
		for (int j = 0, size2 = mOutGrammars[i].PT.size(); j < size2; j++)
		{
			for (int k = 0, size3 = mGrammarForTest.PT.size(); k < size3; k++)
			{
				if (mOutGrammars[i].PT[j].condition == mGrammarForTest.PT[k].condition && !(mOutGrammars[i].PT[j].action == mGrammarForTest.PT[k].action) && !(mOutGrammars[i].PT[j].action == mOutGrammars[i].Start) && !(mGrammarForTest.PT[k].action == mGrammarForTest.Start))//we wrzucanej gramatyce jest pasujący TCl o innym nsymbolu
				{
					NSymbol oldSymbol = mOutGrammars[i].PT[j].action.symbol;
					NSymbol newSymbol = mGrammarForTest.PT[k].action.symbol;
					mOutGrammars[i].PT[j].action.symbol = mGrammarForTest.PT[k].action.symbol;
					for (int l = 0, size4 = mOutGrammars[i].PN.size(); l < size4; l++)
					{
						NClassifier& cl = mOutGrammars[i].PN[l];
						if (cl.action.symbol == oldSymbol)
						{
							cl.action.symbol = newSymbol;
						}
						if (cl.condition.firstSymbol == oldSymbol)
						{
							cl.condition.firstSymbol = newSymbol;
						}
						if (cl.condition.secondSymbol == oldSymbol)
						{
							cl.condition.secondSymbol = newSymbol;
						}
					}
				}
			}
		}
		for (int j = 0, size2 = mOutGrammars[i].PT.size(); j < size2; j++)
		{
			mGrammarForTest.addClNormal(mOutGrammars[i].PT[j]);
		}
		//sprawdzić tak samo ncl'e
		for (int j = 0, size2 = mOutGrammars[i].PN.size(); j < size2; j++)
		{
			for (int k = 0, size3 = mGrammarForTest.PN.size(); k < size3; k++)
			{
				if (mOutGrammars[i].PN[j].condition == mGrammarForTest.PN[k].condition && !(mOutGrammars[i].PN[j].action == mGrammarForTest.PN[k].action) && !(mOutGrammars[i].PN[j].action == mOutGrammars[i].Start) && !(mGrammarForTest.PN[k].action == mGrammarForTest.Start))//we wrzucanej gramatyce jest pasujący NCl o innym nsymbolu
				{
					NSymbol oldSymbol = mOutGrammars[i].PN[j].action.symbol;
					NSymbol newSymbol = mGrammarForTest.PN[k].action.symbol;
					mOutGrammars[i].PN[j].action.symbol = mGrammarForTest.PN[k].action.symbol;
					for (int l = 0, size4 = mOutGrammars[i].PN.size(); l < size4; l++)
					{
						NClassifier& cl = mOutGrammars[i].PN[l];
						if (cl.action.symbol == oldSymbol)
						{
							cl.action.symbol = newSymbol;
						}
						if (cl.condition.firstSymbol == oldSymbol)
						{
							cl.condition.firstSymbol = newSymbol;
						}
						if (cl.condition.secondSymbol == oldSymbol)
						{
							cl.condition.secondSymbol = newSymbol;
						}
					}
				}
			}
		}
		for (int j = 0, size2 = mOutGrammars[i].PN.size(); j < size2; j++)
		{
			mGrammarForTest.addClNormal(mOutGrammars[i].PN[j]);
		}
	}
	mOutGrammars.clear();
	qDebug() << "merging ended\n" << mGrammarForTest.toString();
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

void MainWindow::loadLearningSet()
{
	//	qDebug() << QString() + __FUNCTION__ + " start";
	mLearningSetPositive.clear();
	mLearningSetNegative.clear();
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
					mLearningSetNegative << snt;
				}
				else
				{
					snt.isPositive = true;
					mLearningSetPositive << snt;
				}
				mLearningSetAll << snt;
			}
		}
		file.close();
	}
	listSentences->clear();
	for (int i = 0, size = mLearningSetPositive.size(); i < size; i++)
	{
		listSentences->addItem(QString::number(i + 1) + ": " + mLearningSetPositive[i].toString());
	}
	for (int i = 0, size = mLearningSetNegative.size(); i < size; i++)
	{
		listSentences->addItem(QString::number(i + 1) + ": " + mLearningSetNegative[i].toString());
	}
	//TODO obsługa błędów z plikiem
	//	qDebug() << QString() + __FUNCTION__ + " end";
}

void MainWindow::loadTestingSet()
{
	//	qDebug() << QString() + __FUNCTION__ + " start";
	mTestingSet.clear();
	QFile file(edtFilePathTest->text());
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
				mTestingSet << snt;
			}
		}
		file.close();
	}
	listSentencesTest->clear();
	for (int i = 0, size = mTestingSet.size(); i < size; i++)
	{
		listSentencesTest->addItem(QString::number(i + 1) + ": " + mTestingSet[i].toString());
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

void MainWindow::learningSetFileBrowse()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Open file with sentences"), ".", tr("All files (*)"));
	edtFilePath->setText(file);
}

void MainWindow::testingSetFileBrowse()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Open file with sentences"), ".", tr("All files (*)"));
	edtFilePathTest->setText(file);
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
	//	stepProgressBar->setMaximum(p.maxEvolutionSteps);
	//	iterProgressBar->setMaximum(p.iterations);
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
