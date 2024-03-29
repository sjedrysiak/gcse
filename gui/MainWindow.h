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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include "../ui_MainWindow.h"
#include "SettingsDialog.h"
#include "../src/GCS.h"
#include <QMutex>

class MainWindow: public QMainWindow, private Ui::MainWindowClass
{
	Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    void sendRules(QList<NClassifier> list);
	~MainWindow();
protected slots:
	void about();
	void runGCS();
	void runTest();
	void gcsFinished();
	void testFinished();
	void showSettingsDialog();
	void initGrammar();
    void loadLearningSet();
    void loadTestingSet();
    void changeNonterminalsAmount(int value);
    void changeStartRulesAmount(int value);
    void changeThreadsAmount(int value);
    void changeIterationsAmount(int value);
    void changeMaxEvolutionSteps(int value);
    void changeEndOnFullFintess(bool value);
    void addNonterminal();
    void addRule();
    void clearGrammar();
    void learningSetFileBrowse();
    void testingSetFileBrowse();
private:
	void mergeGrammars();
	void reloadCombos();
	void setupActions();
    void initValues();
    void prepareBars(GCS* thread);
	QList<GCS*> gcsList;
	GCS* gcsTest;
	QList<QProgressBar*> barsList;
	QVBoxLayout* barsLayout;
	SettingsDialog* mSettingsDialog;
	QList<Sentence> mLearningSetAll;
	QList<Sentence> mLearningSetPositive;
	QList<Sentence> mLearningSetNegative;
	QList<Sentence> mTestingSet;
	QList<Grammar> mOutGrammars;
	Grammar mGrammar;
	Grammar mGrammarForTest;
	bool isTestGrammarValid;
	QMutex mutex;
};

#endif // MAINWINDOW_H
