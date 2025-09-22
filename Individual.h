#pragma once
#include <vector>
#include <random>
#include <iostream>
#include "GroupingEvaluator.h"

using namespace NGroupingChallenge;

class Individual {
public:
	Individual(std::vector<int>& genotype, CGroupingEvaluator* evaluator);
	Individual(const Individual& other);
	Individual(Individual&& other) noexcept;
	~Individual() = default;

	Individual& operator=(const Individual& other);
	Individual& operator=(Individual&& other) noexcept;

	double evaluate();
	std::pair<Individual, Individual> cross(const Individual& other) const;
	void mutate(const double& probability, const int& nrOfGroups);

	void print();

private:
	std::vector<int> genotype;
	CGroupingEvaluator* evaluator;
};