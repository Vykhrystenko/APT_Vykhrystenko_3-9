#pragma once

#include <memory>
#include <string>

struct PriorityInput {
    double importance;
    double urgency;
    double waitingTimeMinutes;
};

class IPriorityStrategy {
public:
    virtual ~IPriorityStrategy() = default;

    virtual double calculate(
        const PriorityInput& input
    ) const = 0;

    virtual std::string name() const = 0;
};

class MaximumPriorityStrategy : public IPriorityStrategy {
public:
    double calculate(
        const PriorityInput& input
    ) const override;

    std::string name() const override;
};

class WeightedSumStrategy : public IPriorityStrategy {
public:
    double calculate(
        const PriorityInput& input
    ) const override;

    std::string name() const override;
};

class NormalizedWeightedSumStrategy : public IPriorityStrategy {
public:
    double calculate(
        const PriorityInput& input
    ) const override;

    std::string name() const override;
};

class PriorityContext {
private:
    std::unique_ptr<IPriorityStrategy> strategy;

public:
    explicit PriorityContext(
        std::unique_ptr<IPriorityStrategy> initialStrategy
    );

    void setStrategy(
        std::unique_ptr<IPriorityStrategy> newStrategy
    );

    double calculate(const PriorityInput& input) const;

    std::string strategyName() const;
};


class PriorityStrategyFactory {
public:
    virtual ~PriorityStrategyFactory() = default;

    virtual std::unique_ptr<IPriorityStrategy>
        createStrategy() const = 0;
};

class MaximumPriorityFactory : public PriorityStrategyFactory {
public:
    std::unique_ptr<IPriorityStrategy>
        createStrategy() const override;
};

class WeightedSumFactory : public PriorityStrategyFactory {
public:
    std::unique_ptr<IPriorityStrategy>
        createStrategy() const override;
};

class NormalizedWeightedSumFactory : public PriorityStrategyFactory {
public:
    std::unique_ptr<IPriorityStrategy>
        createStrategy() const override;
};