#include "Individual.h"

Individual::Individual(std::vector<int>& genotype, CGroupingEvaluator* evaluator)
	: genotype(genotype),
	  evaluator(evaluator)
{}

Individual::Individual(const Individual& other)
	: genotype(other.genotype),
	  evaluator(other.evaluator)
{}

Individual::Individual(Individual&& other) noexcept
	: genotype(std::move(other.genotype)),
	  evaluator(other.evaluator)
{
	other.evaluator = nullptr;
}

Individual& Individual::operator=(const Individual& other) {
	if (this != &other) {
		genotype = other.genotype;
		evaluator = other.evaluator;
	}
	return *this;
}

Individual& Individual::operator=(Individual&& other) noexcept {
	if (this != &other) {
		genotype = std::move(other.genotype);
		evaluator = other.evaluator;
		other.evaluator = nullptr;
	}
	return *this;
}

double Individual::evaluate() {
	return evaluator->dEvaluate(genotype);
}

std::pair<Individual, Individual> Individual::cross(const Individual& other) const {
	std::random_device seedGenerator;
	std::mt19937 randomEngine;
	randomEngine.seed(seedGenerator());
	std::uniform_int_distribution<int> distribution(1, genotype.size() - 1);

	int sliceIndex = distribution(randomEngine);

	std::vector<int> genotype1;
	genotype1.insert(genotype1.end(), genotype.begin(), genotype.begin() + sliceIndex);
	genotype1.insert(genotype1.end(), other.genotype.begin() + sliceIndex, other.genotype.end());

	std::vector<int> genotype2;
	genotype2.insert(genotype2.end(), other.genotype.begin(), other.genotype.begin() + sliceIndex);
	genotype2.insert(genotype2.end(), genotype.begin() + sliceIndex, genotype.end());

	Individual child1(genotype1, evaluator);
	Individual child2(genotype2, evaluator);

	return std::make_pair(child1, child2);
}

void Individual::mutate(const double& probability, const int& numberOfGroups) {
	std::random_device seedGenerator;
	std::mt19937 randomEngine;
	randomEngine.seed(seedGenerator());
	std::uniform_real_distribution<double> realDistribution(0.0, 1.0);
	std::uniform_int_distribution<int> intDistribution(1, numberOfGroups);

	for (int i = 0; i < genotype.size(); ++i) {
		if (realDistribution(randomEngine) < probability) {
			genotype[i] = intDistribution(randomEngine);
		}
	}
}

void Individual::print() {
	for (int i = 0; i < genotype.size(); ++i) {
		std::cout << genotype[i];
	}
	std::cout << "\n";
}