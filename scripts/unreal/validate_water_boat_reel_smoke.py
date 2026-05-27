import math

import unreal


MAP_PATH = "/Game/DockShield/Maps/M_Test_Targeting"

LEVEL_EDITOR = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTOR_EDITOR = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)


def fail(message):
    raise RuntimeError(f"DockShield water/boat/Reel smoke validation failed: {message}")


def require_class(name):
    cls = getattr(unreal, name, None)
    if cls is None:
        fail(f"missing reflected class unreal.{name}")
    return cls


def distance(a, b):
    return math.sqrt(
        ((a.x - b.x) ** 2)
        + ((a.y - b.y) ** 2)
        + ((a.z - b.z) ** 2)
    )


def main():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        fail(f"missing map asset {MAP_PATH}")

    LEVEL_EDITOR.load_level(MAP_PATH)

    water_cls = require_class("DSWaterZoneActor")
    boat_cls = require_class("DSPrototypeBoatActor")
    reel_cls = require_class("DSReelPrototypeCharacter")

    spawned = []

    try:
        water = ACTOR_EDITOR.spawn_actor_from_class(
            water_cls,
            unreal.Vector(0.0, -700.0, 0.0),
            unreal.Rotator(0.0, 0.0, 0.0),
        )
        boat = ACTOR_EDITOR.spawn_actor_from_class(
            boat_cls,
            unreal.Vector(520.0, -700.0, -80.0),
            unreal.Rotator(0.0, 0.0, 0.0),
        )
        character = ACTOR_EDITOR.spawn_actor_from_class(
            reel_cls,
            unreal.Vector(0.0, -700.0, 120.0),
            unreal.Rotator(0.0, 0.0, 0.0),
        )
        spawned.extend([water, boat, character])

        for actor, name in [(water, "water"), (boat, "boat"), (character, "character")]:
            if actor is None:
                fail(f"could not spawn {name} actor")

        for method_name in [
            "advance_flood",
            "get_current_velocity_at_location",
            "get_flood_pressure_at_location",
            "get_reel_tension_pressure_at_location",
            "get_water_state_text_at_location",
        ]:
            if not hasattr(water, method_name):
                fail(f"DSWaterZoneActor missing Python method {method_name}")

        for method_name in [
            "apply_water_forces",
            "get_last_drift_distance",
            "get_flood_pressure",
            "get_water_current_speed",
        ]:
            if not hasattr(boat, method_name):
                fail(f"DSPrototypeBoatActor missing Python method {method_name}")

        sample_point = unreal.Vector(0.0, -700.0, -160.0)
        depth = water.get_depth_at_location(sample_point)
        if abs(depth - 160.0) > 0.5:
            fail(f"expected 160cm water depth, got {depth}")

        movement_scale = water.get_movement_scale_at_location(sample_point)
        if movement_scale <= 0.0 or movement_scale >= 1.0:
            fail(f"expected water movement slowdown, got scale {movement_scale}")

        water.set_editor_property("storm_pressure", 0.55)
        water.set_editor_property("current_speed", 180.0)
        water.set_editor_property("flood_surge_rate", 12.0)
        water.set_editor_property("max_water_surface_z", 60.0)
        surface_before = water.get_editor_property("water_surface_z")
        water.advance_flood(1.5)
        surface_after = water.get_editor_property("water_surface_z")
        if surface_after <= surface_before:
            fail("flood surge should raise the water surface")

        current_velocity = water.get_current_velocity_at_location(sample_point)
        current_speed = distance(current_velocity, unreal.Vector(0.0, 0.0, 0.0))
        if current_speed <= 0.0:
            fail("water current should produce velocity at submerged sample point")

        flood_pressure = water.get_flood_pressure_at_location(sample_point)
        reel_pressure = water.get_reel_tension_pressure_at_location(sample_point)
        if flood_pressure <= 0.0 or reel_pressure <= flood_pressure * 0.4:
            fail(f"expected meaningful flood/Reel pressure, got flood={flood_pressure}, reel={reel_pressure}")
        if "CURRENT" not in water.get_water_state_text_at_location(sample_point) and "SURGE" not in water.get_water_state_text_at_location(sample_point):
            fail(f"unexpected water state text: {water.get_water_state_text_at_location(sample_point)}")

        if not water.can_boat_operate_at_location(sample_point):
            fail("boat should operate at 160cm water depth")

        shallow_point = unreal.Vector(0.0, -700.0, -30.0)
        if water.can_boat_operate_at_location(shallow_point):
            fail("boat should not operate at 30cm water depth")

        boat.set_water_depth(depth)
        if not boat.can_float():
            fail("prototype boat should float at validated water depth")

        drift_start = boat.get_actor_location()
        if not boat.apply_water_forces(current_velocity, flood_pressure, 1.0):
            fail("floating boat should drift under water current")
        drift_end = boat.get_actor_location()
        if boat.get_last_drift_distance() <= 0.0:
            fail("boat reported no water drift distance")
        if distance(drift_start, drift_end) <= 1.0:
            fail("water current did not move the boat")
        if boat.get_flood_pressure() <= 0.0 or boat.get_water_current_speed() <= 0.0:
            fail("boat did not retain flood/current telemetry")

        before_distance = distance(character.get_actor_location(), boat.get_actor_location())
        if not character.execute_reel_action_on_target(boat):
            fail("The Reel did not tow the prototype boat")
        after_distance = distance(character.get_actor_location(), boat.get_actor_location())

        if character.get_boat_tow_count() != 1:
            fail(f"expected one boat tow, got {character.get_boat_tow_count()}")
        if boat.get_last_tow_distance() <= 0.0:
            fail("boat reported no tow distance")
        if after_distance >= before_distance:
            fail(f"boat was not pulled closer: before={before_distance:.1f}, after={after_distance:.1f}")

        if not boat.board_boat(character):
            fail("character should board a floating empty boat")
        if boat.board_boat(character):
            fail("occupied boat should reject a second board action")

        if "OCCUPIED" not in boat.get_boat_state_text():
            fail(f"expected occupied boat state, got {boat.get_boat_state_text()}")

        seat_distance = distance(character.get_actor_location(), boat.get_seat_world_location())
        if seat_distance > 5.0:
            fail(f"boarded character was not moved to the boat seat, distance={seat_distance:.1f}")

        pilot_start = boat.get_actor_location()
        if not boat.apply_pilot_input(unreal.Vector2D(0.0, 1.0), 0.0, 0.25):
            fail("occupied floating boat should accept pilot movement input")
        pilot_end = boat.get_actor_location()
        if boat.get_last_pilot_distance() <= 0.0:
            fail("boat reported no pilot movement distance")
        if distance(pilot_start, pilot_end) <= 1.0:
            fail("pilot input did not move the boat")

        boat.exit_boat()
        exit_state = boat.get_boat_state_text()
        if "OCCUPIED" in exit_state or "TOO SHALLOW" in exit_state:
            fail(f"expected empty floating boat after exit, got {exit_state}")

        if not hasattr(character, "try_board_or_exit_boat"):
            fail("Reel character is missing board/exit input method")
        if not hasattr(character, "is_boarded_boat"):
            fail("Reel character is missing boarded-state accessor")
        if not hasattr(character, "get_boat_status_text"):
            fail("Reel character is missing boat status HUD accessor")

        boat.set_water_depth(30.0)
        if boat.apply_reel_tow_from(character.get_actor_location(), 280.0):
            fail("boat tow should fail in shallow water")

    finally:
        for actor in reversed(spawned):
            if actor:
                ACTOR_EDITOR.destroy_actor(actor)

    unreal.log("DockShield water/boat/Reel smoke validation passed: depth, slowdown, floating, towing, boarding, and piloting.")


main()
