#ifndef SELECTION_OPERATOR_H
#define SELECTION_OPERATOR_H

#include "GA_Operator.h"

class SelectionOperator : GA_Operator {

};

class RouletteSelection : SelectionOperator {

};

class TournamentSelection : SelectionOperator {

};

class SUS_Selection : SelectionOperator {

	SUS

	// (float weakest_fitness, float average_fitness);
};

#endif
