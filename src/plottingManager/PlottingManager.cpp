#include "PlottingManager.hpp"

#include "config/pins.hpp"
#include "systemServices/Queues.hpp"

PlottingManager::PlottingManager(MotionStateManager& ms)
    : ms(ms),
    driverSerial(1),

    rawDriverA(&driverSerial, R_SENSE, 0),
    rawDriverB(&driverSerial, R_SENSE, 2),

    driverA(rawDriverA),
    driverB(rawDriverB),

    stepA(STEP_PIN_A, DIR_PIN_A),
    stepB(STEP_PIN_B, DIR_PIN_B),

    axisA(stepA, driverA, true),
    axisB(stepB, driverB, true),

    pen(penServo, PEN_UP_ANGLE, PEN_DOWN_ANGLE),

    homingController(
        axisA,
        axisB,
        driverA,
        driverB,
        HOMING_SPEED_STPS_PER_S,
        HOMING_STALLGUARD_THRESHOLD,
        HOMING_SG_CHECK_INTERVAL_MS,
        HOMING_CONSECUTIVE_STALL_CHECKS,
        HOMING_SG_START_TIMEOUT_MS
    ),

    kinematics(STEPS_PER_MM),

    motionSystem(axisA, axisB, kinematics, ms, MIN_FEATURE_SIZE_MM),

    gcodeParser(
        motionSystem,
        pen,
        homingController,
        FEED_RATE_DRAW_MM_PER_S,
        FEED_RATE_TRAVEL_MM_PER_S,
        ms
    )
{
}

void PlottingManager::configureDriver(TMC2209Driver& driver)
{
    driver.begin();
    driver.setStallGuardThreshold(HOMING_STALLGUARD_THRESHOLD);
    driver.setCurrent(CURRENT_MA);
    driver.setMicrosteps(MICROSTEPS);
}

void PlottingManager::init()
{
    pinMode(SERVO_PIN, OUTPUT);

    penServo.attach(SERVO_PIN);
    pen.up();

    pinMode(EN_PIN, OUTPUT);
    digitalWrite(EN_PIN, LOW);

    driverSerial.begin(115200, SERIAL_8N1, DRIVER_RX_PIN, DRIVER_TX_PIN);

    configureDriver(driverA);
    configureDriver(driverB);

    Serial.println("Plotting manager initialized.");
}

void PlottingManager::update()
{
    GcodeMessage msg;

    if (xQueueReceive(gcodeQueue, &msg, 0) == pdTRUE && ms.getCommand() != MotionCommand::ABORT)
    {
        ms.setState(MotionState::RUNNING);

        Serial.print("Executing line: ");
        Serial.println(msg.line);

        gcodeParser.executeLine(msg.line);
    }
    else
    {
        ms.setState(MotionState::IDLE);
    }
}