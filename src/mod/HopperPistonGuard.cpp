#include "mod/HopperPistonGuard.h"

#include <excpt.h>
#include <string>

#include "ll/api/io/Logger.h"
#include "ll/api/memory/Hook.h"
#include "ll/api/mod/NativeMod.h"

#include "mc/world/level/block/Block.h"
#include "mc/world/level/block/components/BlockMovableComponent.h"
#include "mc/world/level/block/components/MovementType.h"

// LL_TYPE_*_HOOK 手动注册宏会生成未被引用的 _AutoHookCount，属预期行为。
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#endif

namespace hopper_piston_guard {
namespace {

ll::io::Logger& logger() { return ll::mod::NativeMod::current()->getLogger(); }

/// 判断方块是否为漏斗。引擎查询“可移动性”时我们把它伪装成不可移动，
/// 与黑曜石/基岩同一机制：普通与粘性活塞都无法推动/拉动它。
[[nodiscard]] bool isHopperBlock(::Block const& block) {
    __try {
        ::std::string const& name = block.getTypeName();
        return name == "minecraft:hopper" || name == "hopper" || name.ends_with(":hopper");
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

LL_TYPE_STATIC_HOOK(
    BlockMovableComponentGetMovementTypeHook,
    HookPriority::Normal,
    ::BlockMovableComponent,
    &::BlockMovableComponent::getMovementType,
    ::MovementType,
    ::Block const& block
) {
    if (isHopperBlock(block)) {
        logger().debug("已阻止活塞移动漏斗（按不可移动方块处理）");
        return ::MovementType::Immovable;
    }
    return origin(block);
}

} // namespace

bool enableHopperPistonGuard() {
    if (BlockMovableComponentGetMovementTypeHook::hook() != 0) {
        logger().error("注册钩子 BlockMovableComponentGetMovementTypeHook 失败");
        return false;
    }
    logger().info("HopperPistonGuard 已启用：漏斗不可被活塞推动或拉动。");
    return true;
}

void disableHopperPistonGuard() {
    BlockMovableComponentGetMovementTypeHook::unhook();
    logger().info("HopperPistonGuard 已停用。");
}

} // namespace hopper_piston_guard

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
