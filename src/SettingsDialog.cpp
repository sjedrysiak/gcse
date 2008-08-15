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

#include "SettingsDialog.h"
#include "Params.h"

#include <QtCore>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
	this->setupUi(this);
	this->setupActions();
}

void SettingsDialog::setupActions()
{
	this->updateFromParams();
}

void SettingsDialog::accept()
{
	try
	{
		Params::setAllowCorrection(this->cbxGrammarCorrection->isChecked());
		Params::setAllowCoveringStart(this->cbxStartCovering->isChecked());
		Params::setAllowCoveringFull(this->cbxFullCovering->isChecked());
		Params::setAllowCoveringUniversal(this->cbxAllowUniversalSymbol->isChecked());
		Params::setCoveringAggressiveProb(this->sbxAggressiveCovering->value());
		Params::setAllowGA(this->cbxAllowGA->isChecked());
		GA::SelectionType sel1;
		if (this->rbnRandom1->isChecked())
		{
			sel1 = GA::RANDOM;
		}
		else if (this->rbnRoulette1->isChecked())
		{
			sel1 = GA::ROULETTE;
		}
		else
		{
			sel1 = GA::TOURNAMENT;
		}
		GA::SelectionType sel2;
		if (this->rbnRandom2->isChecked())
		{
			sel2 = GA::RANDOM;
		}
		else if (this->rbnRoulette2->isChecked())
		{
			sel2 = GA::ROULETTE;
		}
		else
		{
			sel2 = GA::TOURNAMENT;
		}
		Params::setSelectionCl1(sel1);
		Params::setSelectionCl2(sel2);
		Params::setCrossoverProb(this->sbxCrossoverProb->value());
		Params::setMutationProb(this->sbxMutationProb->value());
		Params::setInversionProb(this->sbxInversionProb->value());
		Params::setEliteSize(this->sbxEliteSize->value());
		Params::setTournamentSize(this->sbxTournamentSubpop->value());
		Params::setCrowdFactor(this->sbxCrowdingFactor->value());
		Params::setCrowdSize(this->sbxCrowdingSubpop->value());
		Params::setBaseAmount(this->sbxBaseAmount->value());
		Params::setRenouncedAmountFactor(this->sbxRAF->value());
		Params::setPopulationSize(this->sbxPopulationSize->value());
		Params::setNonterminalSymbolsAmount(this->sbxNonterminals->value());
		Params::setPositiveSentenceWeight(this->sbxPositiveWeight->value());
		Params::setNegativeSentenceWeight(this->sbxNegativeWeight->value());
		Params::setClassicFunWeight(this->sbxClassicWeight->value());
		Params::setFertilityFunWeight(this->sbxFertilityWeight->value());
		Params::setUnusedClassifierFitness(this->sbxUnusedFitness->value());
	}
	catch(ArgumentOutOfRangeException& exc)
	{
		qDebug() << exc;
		throw exc;
	}
	catch(MyException& exc)
	{
		qDebug() << exc;
		throw exc;
	}
	this->hide();
}

void SettingsDialog::reject()
{
	this->updateFromParams();
	this->hide();
}

void SettingsDialog::updateFromParams()
{
	//read Params and set fields on dialog
}

SettingsDialog::~SettingsDialog()
{
}
