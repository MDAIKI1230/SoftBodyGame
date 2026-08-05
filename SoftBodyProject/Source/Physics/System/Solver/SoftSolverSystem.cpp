#include "ServiceLocator.h"

#include "SoftSolverSystem.h"


void SoftSolverSystem::Initialize()
{
    computeShader = ServiceLocator::GetIGPUConnecter()->LoadComputeShader("Shader/SoftSolverCS.comp");
    struct alignas(16) GPUParticle
    {
        float position[4];
        float velocity[4];
    };

    static_assert(sizeof(GPUParticle) == 32);

    constexpr uint32_t PARTICLE_COUNT = 256;
    constexpr uint32_t COLUMN_COUNT = 16;

    std::vector<GPUParticle> particles(PARTICLE_COUNT);

    for (uint32_t i = 0; i < PARTICLE_COUNT; ++i)
    {
        const uint32_t column = i % COLUMN_COUNT;
        const uint32_t row = i / COLUMN_COUNT;

        const float x = -120.0f + static_cast<float>(column) * 16.0f;
        const float y = -80.0f + static_cast<float>(row) * 12.0f;

        float speed = 20.0f + static_cast<float>(i % 8) * 5.0f;

        if ((row % 2) != 0)
        {
            speed = -speed;
        }

        particles[i] =
        {
            { x, y, 0.0f, 1.0f },
            { speed, 0.0f, 0.0f, 0.0f }
        };
    }

    particleBuffer = ServiceLocator::GetIGPUConnecter()->CreateShaderBuffer(
        static_cast<uint32_t>(sizeof(GPUParticle) * particles.size()),
        particles.data()
    );

    constexpr uint32_t THREAD_COUNT = 64;

    groupCount = (PARTICLE_COUNT + THREAD_COUNT - 1) / THREAD_COUNT;
}

void SoftSolverSystem::Solve(BodyStorage* _bodyStorage)
{
    ServiceLocator::GetIGPUConnecter()->BindShaderBuffer(particleBuffer, 0);

    ServiceLocator::GetIGPUConnecter()->Dispatch(computeShader, groupCount, 1, 1);

    ServiceLocator::GetIGPUConnecter()->ShaderBufferBarrier();
}

SoftSolverSystem::~SoftSolverSystem()
{
    ServiceLocator::GetIGPUConnecter()->DestroyComputeShader(computeShader);
    ServiceLocator::GetIGPUConnecter()->DestroyShaderBuffer(particleBuffer);
}
