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
#include "../src/Params.h"

#include <QtCore>

SettingsDialog::SettingsDialog(QWidget *parent) :
	QDialog(parent)
{
	setupUi(this);
	setupActions();
	updateFromParams();
}

void SettingsDialog::setupActions()
{
}

void SettingsDialog::accept()
{
	Params::instance().maxNonterminalRules = sbxPopulationSize->value();
	Params::instance().nonterminalSymbolsAmount = sbxNonterminals->value();
	Params::instance().unusedClassifierFitness = sbxUnusedFitness->value();

	Params::instance().allowCorrection = cbxGrammarCorrection->isChecked();
	Params::instance().allowCoveringStart = cbxStartCovering->isChecked();
	Params::instance().allowCoveringFull = cbxFullCovering->isChecked();
	Params::instance().allowCoveringUniversal = cbxAllowUniversalSymbol->isChecked();
	Params::instance().coveringAggressiveProb = sbxAggressiveCovering->value();
	Params::instance().allowGA = cbxAllowGA->isChecked();
	GA::SelectionType sel1;
	if (rbnRandom1->isChecked())
	{
		sel1 = GA::RANDOM;
	}
	else if (rbnRoulette1->isChecked())
	{
		sel1 = GA::ROULETTE;
	}
	else
	{
		sel1 = GA::TOURNAMENT;
	}
	GA::SelectionType sel2;
	if (rbnRandom2->isChecked())
	{
		sel2 = GA::RANDOM;
	}
	else if (rbnRoulette2->isChecked())
	{
		sel2 = GA::ROULETTE;
	}
	else
	{
		sel2 = GA::TOURNAMENT;
	}
	Params::instance().selectionParent1 = sel1;
	Params::instance().selectionParent2 = sel2;
	Params::instance().crossoverProb = sbxCrossoverProb->value();
	Params::instance().mutationProb = sbxMutationProb->value();
	Params::instance().inversionProb = sbxInversionProb->value();
	Params::instance().eliteSize = sbxEliteSize->value();
	Params::instance().tournamentSize = sbxTournamentSubpop->value();
	Params::instance().crowdFactor = sbxCrowdingFactor->value();
	Params::instance().crowdSize = sbxCrowdingSubpop->value();
	Params::instance().baseAmount = sbxBaseAmount->value();
	Params::instance().renouncedAmountFactor = sbxRAF->value();
	Params::instance().positiveSentenceWeight = sbxPositiveWeight->value();
	Params::instance().negativeSentenceWeight = sbxNegativeWeight->value();
	Params::instance().classicFunWeight = sbxClassicWeight->value();
	Params::instance().fertilityFunWeight = sbxFertilityWeight->value();
	hide();
}

void SettingsDialog::reject()
{
	updateFromParams();
	hide();
}

void SettingsDialog::updateFromParams()
{
	cbxGrammarCorrection->setChecked(Params::instance().allowCorrection);
	cbxStartCovering->setChecked(Params::instance().allowCoveringStart);
	cbxFullCovering->setChecked(Params::instance().allowCoveringFull);
	cbxAllowUniversalSymbol->setChecked(Params::instance().allowCoveringUniversal);
	sbxAggressiveCovering->setValue(Params::instance().coveringAggressiveProb);
	cbxAllowGA->setChecked(Params::instance().allowGA);
	switch (Params::instance().selectionParent1)
	{
		case GA::RANDOM:
			rbnRandom1->setChecked(true);
			break;
		case GA::ROULETTE:
			rbnRoulette1->setChecked(true);
			break;
		case GA::TOURNAMENT:
			rbnTournament1->setChecked(true);
			break;
	}
	switch (Params::instance().selectionParent2)
	{
		case GA::RANDOM:
			rbnRandom2->setChecked(true);
			break;
		case GA::ROULETTE:
			rbnRoulette2->setChecked(true);
			break;
		case GA::TOURNAMENT:
			rbnTournament2->setChecked(true);
			break;
	}
	sbxCrossoverProb->setValue(Params::instance().crossoverProb);
	sbxMutationProb->setValue(Params::instance().mutationProb);
	sbxInversionProb->setValue(Params::instance().inversionProb);
	sbxEliteSize->setValue(Params::instance().eliteSize);
	sbxTournamentSubpop->setValue(Params::instance().tournamentSize);
	sbxCrowdingFactor->setValue(Params::instance().crowdFactor);
	sbxCrowdingSubpop->setValue(Params::instance().crowdSize);
	sbxBaseAmount->setValue(Params::instance().baseAmount);
	sbxRAF->setValue(Params::instance().renouncedAmountFactor);
	sbxPopulationSize->setValue(Params::instance().maxNonterminalRules);
	sbxNonterminals->setValue(Params::instance().nonterminalSymbolsAmount);
	sbxPositiveWeight->setValue(Params::instance().positiveSentenceWeight);
	sbxNegativeWeight->setValue(Params::instance().negativeSentenceWeight);
	sbxClassicWeight->setValue(Params::instance().classicFunWeight);
	sbxFertilityWeight->setValue(Params::instance().fertilityFunWeight);
	sbxUnusedFitness->setValue(Params::instance().unusedClassifierFitness);
}

SettingsDialog::~SettingsDialog()
{
}
