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
	Params& p = Params::instance();
	p.maxNonterminalRules = sbxMaxNonterminals->value();
	p.unusedClassifierFitness = sbxUnusedFitness->value();

//	p.allowCorrection = cbxGrammarCorrection->isChecked();
	p.allowCoveringStart = cbxStartCovering->isChecked();
	p.allowCoveringFull = cbxFullCovering->isChecked();
//	p.allowCoveringUniversal = cbxAllowUniversalSymbol->isChecked();
	p.coveringAggressiveProb = sbxAggressiveCovering->value();
	p.allowGA = cbxAllowGA->isChecked();
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
	p.selectionParent1 = sel1;
	p.selectionParent2 = sel2;
	p.crossoverProb = sbxCrossoverProb->value();
	p.mutationProb = sbxMutationProb->value();
	p.inversionProb = sbxInversionProb->value();
	p.eliteSize = sbxEliteSize->value();
	p.tournamentSize = sbxTournamentSubpop->value();
	p.crowdFactor = sbxCrowdingFactor->value();
	p.crowdSize = sbxCrowdingSubpop->value();
	p.baseAmount = sbxBaseAmount->value();
	p.renouncedAmountFactor = sbxRAF->value();
	p.positiveSentenceWeight = sbxPositiveWeight->value();
	p.negativeSentenceWeight = sbxNegativeWeight->value();
	p.classicFunWeight = sbxClassicWeight->value();
	p.fertilityFunWeight = sbxFertilityWeight->value();
	p.exchangeProb = sbxExchangeProb->value();
	p.exchangeAmount = sbxExchangeAmount->value();
	hide();
}

void SettingsDialog::reject()
{
	updateFromParams();
	hide();
}

void SettingsDialog::updateFromParams()
{
	Params& p = Params::instance();
//	cbxGrammarCorrection->setChecked(p.allowCorrection);
	cbxStartCovering->setChecked(p.allowCoveringStart);
	cbxFullCovering->setChecked(p.allowCoveringFull);
//	cbxAllowUniversalSymbol->setChecked(p.allowCoveringUniversal);
	sbxAggressiveCovering->setValue(p.coveringAggressiveProb);
	cbxAllowGA->setChecked(p.allowGA);
	switch (p.selectionParent1)
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
	switch (p.selectionParent2)
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
	sbxCrossoverProb->setValue(p.crossoverProb);
	sbxMutationProb->setValue(p.mutationProb);
	sbxInversionProb->setValue(p.inversionProb);
	sbxEliteSize->setValue(p.eliteSize);
	sbxTournamentSubpop->setValue(p.tournamentSize);
	sbxCrowdingFactor->setValue(p.crowdFactor);
	sbxCrowdingSubpop->setValue(p.crowdSize);
	sbxBaseAmount->setValue(p.baseAmount);
	sbxRAF->setValue(p.renouncedAmountFactor);
	sbxMaxNonterminals->setValue(p.maxNonterminalRules);
	sbxPositiveWeight->setValue(p.positiveSentenceWeight);
	sbxNegativeWeight->setValue(p.negativeSentenceWeight);
	sbxClassicWeight->setValue(p.classicFunWeight);
	sbxFertilityWeight->setValue(p.fertilityFunWeight);
	sbxUnusedFitness->setValue(p.unusedClassifierFitness);
	sbxExchangeProb->setValue(p.exchangeProb);
	sbxExchangeAmount->setValue(p.exchangeAmount);
}

SettingsDialog::~SettingsDialog()
{
}
