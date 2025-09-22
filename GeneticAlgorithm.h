#pragma once
#include<vector>
#include "Individual.h"
#include "GroupingEvaluator.h"

using namespace NGroupingChallenge;

class GeneticAlgorithm {
public:
	GeneticAlgorithm(const int& populationSize, const double& crossProbability, const double& mutationProbability, CGroupingEvaluator* evaluator);
	~GeneticAlgorithm();

	void initializePopulation();
	void runAlgorithm();
	Individual* getBestSolution();

private:
	std::vector<Individual> population;
	std::vector<double> scores;
	CGroupingEvaluator* evaluator;
	mt19937 randomEngine;

	int populationSize;
	double crossProbability;
	double mutationProbability;

	Individual* bestSolution;
	double bestFitness;

	void runIteration();
	void evaluatePopulation();
	std::vector<Individual> createNextGeneration();
	Individual chooseParent(uniform_int_distribution<int> distribution);
	void mutatePopulation();

};

