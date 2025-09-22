#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(const int& populationSize, const double& crossProbability, const double& mutationProbability, CGroupingEvaluator* evaluator) 
	: evaluator(evaluator),
	  bestSolution(nullptr),
	  bestFitness(-2)
{
	if (populationSize <= 0) {
		throw std::invalid_argument("Size of population must be greater than 0");
	}
	this->populationSize = populationSize;

	if (crossProbability <= 0 || crossProbability > 1) {
		throw std::invalid_argument("Cross probability must be between 0 and 1");
	}
	this->crossProbability = crossProbability;

	if (mutationProbability <= 0 || mutationProbability >= 1) {
		throw std::invalid_argument("Mutation probability must be between 0 and 1");
	}
	this->mutationProbability = mutationProbability;

	random_device seedGenerator;
	randomEngine.seed(seedGenerator());
}

GeneticAlgorithm::~GeneticAlgorithm() {
	delete bestSolution;
}

void GeneticAlgorithm::initializePopulation() {
	uniform_int_distribution<int> distribution(evaluator->iGetLowerBound(), evaluator->iGetUpperBound());

	for (int i = 0; i < populationSize; ++i) {
		std::vector<int> genotype(evaluator->iGetNumberOfPoints());

		for (int j = 0; j < genotype.size(); j++) {
			genotype[j] = distribution(randomEngine);
		}
		population.emplace_back(genotype, evaluator);
	}
}

void GeneticAlgorithm::runIteration() {
	//obliczanie ewaluacji osobnikow
	evaluatePopulation();
	std::cout << "best score SO FAR: " << bestFitness << "\nbest individual SO FAR: ";
	getBestSolution()->print();
	std::cout << "\n";

	//turniej
	population = createNextGeneration();

	//mutacja
	mutatePopulation();
}

void GeneticAlgorithm::evaluatePopulation() {
	scores.clear();

	double bestScore = population[0].evaluate();
	scores.push_back(bestScore);
	Individual& bestIndividual = population[0];

	for (int i = 1; i < populationSize; ++i) {
		scores.push_back(population[i].evaluate());

		if (scores[i] < bestScore) {
			bestScore = scores[i];
			bestIndividual = population[i];
		}
	}
	/*std::cout << "	best score: " << bestScore << "\n	best individual THIS iteration: ";
	bestIndividual.print();*/

	if (bestSolution == nullptr) {
		bestSolution = new Individual(bestIndividual);
		bestFitness = bestScore;
	}
	else {
		if (bestScore < bestFitness) {
			delete bestSolution;
			bestSolution = new Individual(bestIndividual);
			bestFitness = bestScore;
		}
	}
}

std::vector<Individual> GeneticAlgorithm::createNextGeneration() {
	std::vector<Individual> nextGeneration;

	uniform_int_distribution<int> intDistribution(0, populationSize - 1);
	uniform_real_distribution<double> realDistribution(0, 1);
	for (int i = 0; i < (populationSize + 1) / 2; ++i) {
		Individual parent1 = chooseParent(intDistribution);

		Individual parent2 = chooseParent(intDistribution);

		double prob = realDistribution(randomEngine);
		if (prob < crossProbability) {
			std::pair<Individual, Individual> children = parent1.cross(parent2);
			nextGeneration.push_back(std::move(children.first));
			nextGeneration.push_back(std::move(children.second));
		}
		else {
			nextGeneration.push_back(std::move(parent1));
			nextGeneration.push_back(std::move(parent2));
		}
	}

	return nextGeneration;
}

Individual GeneticAlgorithm::chooseParent(uniform_int_distribution<int> distribution) {
	int potentialParentIndex1 = distribution(randomEngine);
	int potentialParentIndex2 = distribution(randomEngine);

	if (scores[potentialParentIndex1] < scores[potentialParentIndex2]) {
		return population[potentialParentIndex1];
	}
	else {
		return population[potentialParentIndex2];
	}
}

void GeneticAlgorithm::mutatePopulation() {
	for (int i = 0; i < populationSize; ++i) {
		population[i].mutate(mutationProbability, evaluator->iGetUpperBound());
	}
}

void GeneticAlgorithm::runAlgorithm() {
	for (int i = 0; i < 10; ++i) {
		runIteration();
	}
}

Individual* GeneticAlgorithm::getBestSolution() {
	return bestSolution;
}