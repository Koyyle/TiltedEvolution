#include "ActorMagicCaster.h"

#include <Events/SpellCastEvent.h>
#include <Events/InterruptCastEvent.h>

#include <Games/Skyrim/Actor.h>
#include <Games/ActorExtension.h>

#include <World.h>

TP_THIS_FUNCTION(TSpellCast, void, ActorMagicCaster, bool sbSuccess, int32_t auiTargetCount, MagicItem* apSpell);
TP_THIS_FUNCTION(TInterruptCast, void, ActorMagicCaster, bool);

static TSpellCast* RealSpellCast = nullptr;
static TInterruptCast* RealInterruptCast = nullptr;

void TP_MAKE_THISCALL(HookSpellCast, ActorMagicCaster, bool abSuccess, int32_t auiTargetCount, MagicItem* apSpell)
{
    if (!abSuccess)
        return;

    Actor* pActor = apThis->pCasterActor;

    if (pActor->GetExtension()->IsRemote())
        return;

    uint32_t targetFormId = 0;

    if (apThis->hDesiredTarget)
    {
        TESObjectREFR* pDesiredTarget = TESObjectREFR::GetByHandle(apThis->hDesiredTarget.handle.iBits);
        if (pDesiredTarget)
        {
            targetFormId = pDesiredTarget->formID;
        }
    }

    World::Get().GetRunner().Trigger(SpellCastEvent(apThis, apSpell, targetFormId));

    spdlog::info("HookSpellCast, abSuccess: {}, auiTargetCount: {}, apSpell: {:X}, desired target: {:X}", abSuccess, auiTargetCount, (uint64_t)apSpell, targetFormId);

    ThisCall(RealSpellCast, apThis, abSuccess, auiTargetCount, apSpell);
}

void TP_MAKE_THISCALL(HookInterruptCast, ActorMagicCaster, bool abRefund)
{
    ActorExtension* pExtended = apThis->pCasterActor->GetExtension();

    if (pExtended->IsLocal())
    {
        World::Get().GetRunner().Trigger(InterruptCastEvent(apThis->pCasterActor->formID));
    }

    ThisCall(RealInterruptCast, apThis, abRefund);
}

static TiltedPhoques::Initializer s_actorMagicCasterHooks([]() {
    POINTER_SKYRIMSE(TSpellCast, s_spellCast, 0x14055CB90 - 0x140000000);
    POINTER_SKYRIMSE(TInterruptCast, s_interruptCast, 0x14055C470 - 0x140000000);

    RealSpellCast = s_spellCast.Get();
    RealInterruptCast = s_interruptCast.Get();

    TP_HOOK(&RealSpellCast, HookSpellCast);
    TP_HOOK(&RealInterruptCast, HookInterruptCast);
});
