#ifndef RANDOMSYSTEM_H
#define RANDOMSYSTEM_H

#include<random>

#define RANDOMSYSTEM RandomSystem::GetInstance().GetRandomEngine()

class RandomSystem
{
public:
    static RandomSystem& GetInstance()noexcept
    {
		static RandomSystem randomSystem;
		return randomSystem;
	}

    RandomSystem(RandomSystem const&) = delete;
    void operator=(RandomSystem const&) = delete;

    inline std::mt19937 const& GetRandomEngine()const noexcept{ return m_RandomEngine; }
    inline std::mt19937 & GetRandomEngine() noexcept{ return m_RandomEngine; }
    inline void SetRandomEngine(std::mt19937 const& randomEngine) noexcept{ m_RandomEngine = randomEngine; }

protected:
    RandomSystem()noexcept: m_RandomEngine(std::random_device{}()){}

private:
    std::mt19937 m_RandomEngine;
};

#endif // RANDOMSYSTEM_H
