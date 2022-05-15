#pragma once

#include "PointerObjectWrapper.h"
#include "../Core/Math.h"

#include <stdint.h>

namespace Sky
{
    namespace Game
    {

        class BaseEntity : public PointerObjectWrapper
        {
        public:
            using PointerObjectWrapper::PointerObjectWrapper;

            static constexpr uintptr_t GetObjectSize()
            {
                return 0x10;
            }

#pragma region General
            uint8_t GetHealth() const;
            uint8_t GetTeam() const;
#pragma endregion

#pragma region Position
            Vector3f GetViewOffset() const;

            Vector3f GetAbsolutePosition() const;
            Vector3f GetHeadPosition() const;
            Vector3f GetFeetPosition() const;

            //Vector3f GetBonePosition(uint32_t targetBone);

            Vector3f GetVelocity();
#pragma endregion

#pragma region Weapons
            uint32_t GetWeaponHandle() const;
            //uint32_t GetCurrentWeapon();
            uint16_t GetCurrentWeaponId() const;
            uint16_t GetWeaponIndex() const;

            uint16_t GetCrosshairId() const;
            uint16_t GetShotsFired() const;

            Vector3f GetAimPunch() const;

            uint8_t GetForceAttack() const;
#pragma endregion

#pragma region Visuals
            uint32_t GetGlowIndex() const;
            void SetCorrectGlowStruct(uint8_t localPlayerTeam, uint32_t glowObject) const;

            void SetFlashAlpha(float num) const;
#pragma endregion

#pragma region Actions
            void SetForceJump(uint8_t value) const;
            void SetForceAttackPrimary(uint8_t value) const;
            void SetForceAttackSecondary(uint8_t value) const;
            void Shoot();
#pragma endregion

#pragma region Flags
            uint32_t GetFlags() const;

            bool IsValidEntity() const;
            bool IsDormant() const;
            bool IsDefusing() const;
            bool IsInAir() const;
            bool IsPlayer() const;
            bool IsCrouching() const;
#pragma endregion

        };

    }
}