#include "PriorityStrategy.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace {

    void validateInput(const PriorityInput& input) {
        if (!std::isfinite(input.importance) ||
            !std::isfinite(input.urgency) ||
            !std::isfinite(input.waitingTimeMinutes)) {

            throw std::invalid_argument(
                "Priority parameters must be finite numbers."
            );
        }

        if (input.importance < 1 || input.importance > 10) {
            throw std::invalid_argument(
                "Importance must be between 1 and 10."
            );
        }

        if (input.urgency < 1 || input.urgency > 10) {
            throw std::invalid_argument(
                "Urgency must be between 1 and 10."
            );
        }

        if (input.waitingTimeMinutes < 0 ||
            input.waitingTimeMinutes > 60) {

            throw std::invalid_argument(
                "Waiting time must be between 0 and 60 minutes."
            );
        }
    }

    double calculateWaitingScore(const PriorityInput& input) {
        return input.waitingTimeMinutes / 6.0;
    }

}


double MaximumPriorityStrategy::calculate(
    const PriorityInput& input
) const {
    validateInput(input);

    const double waitingScore =
        calculateWaitingScore(input);

    return std::max({
        input.importance,
        input.urgency,
        waitingScore
        });
}

std::string MaximumPriorityStrategy::name() const {
    return "Maximum value strategy";
}

double WeightedSumStrategy::calculate(
    const PriorityInput& input
) const {
    validateInput(input);

    const double waitingScore =
        calculateWaitingScore(input);

    return 0.5 * input.importance
        + 0.3 * input.urgency
        + 0.2 * waitingScore;
}

std::string WeightedSumStrategy::name() const {
    return "Weighted sum strategy";
}

double NormalizedWeightedSumStrategy::calculate(
    const PriorityInput& input
) const {
    validateInput(input);

    return 0.5 * (input.importance / 10.0)
        + 0.3 * (input.urgency / 10.0)
        + 0.2 * (input.waitingTimeMinutes / 60.0);
}

std::string NormalizedWeightedSumStrategy::name() const {
    return "Normalized weighted sum strategy";
}

PriorityContext::PriorityContext(
    std::unique_ptr<IPriorityStrategy> initialStrategy
) {
    setStrategy(std::move(initialStrategy));
}

void PriorityContext::setStrategy(
    std::unique_ptr<IPriorityStrategy> newStrategy
) {
    if (!newStrategy) {
        throw std::invalid_argument(
            "Priority strategy cannot be null."
        );
    }

    strategy = std::move(newStrategy);
}

double PriorityContext::calculate(
    const PriorityInput& input
) const {
    return strategy->calculate(input);
}

std::string PriorityContext::strategyName() const {
    return strategy->name();
}

std::unique_ptr<IPriorityStrategy>
MaximumPriorityFactory::createStrategy() const {
    return std::make_unique<MaximumPriorityStrategy>();
}

std::unique_ptr<IPriorityStrategy>
WeightedSumFactory::createStrategy() const {
    return std::make_unique<WeightedSumStrategy>();
}

std::unique_ptr<IPriorityStrategy>
NormalizedWeightedSumFactory::createStrategy() const {
    return std::make_unique<NormalizedWeightedSumStrategy>();
}