#include "mod/MyMod.h"

#include "mod/HopperPistonGuard.h"

#include "ll/api/mod/RegisterHelper.h"

namespace hopper_piston_guard {

MyMod& MyMod::getInstance() {
    static MyMod instance;
    return instance;
}

bool MyMod::load() {
    getSelf().getLogger().debug("Loading...");
    // Code for loading the mod goes here.
    return true;
}

bool MyMod::enable() {
    if (!enableHopperPistonGuard()) return false;
    return true;
}

bool MyMod::disable() {
    disableHopperPistonGuard();
    return true;
}

} // namespace hopper_piston_guard

LL_REGISTER_MOD(hopper_piston_guard::MyMod, hopper_piston_guard::MyMod::getInstance());
