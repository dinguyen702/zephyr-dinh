/*
 * Copyright (c) 2021 Leica Geosystems AG
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/zephyr.h>
#include <zephyr/ztest.h>
#include <zephyr/drivers/sensor.h>

const struct device *get_fuel_gauge_device(void)
{
	const struct device *const dev = DEVICE_DT_GET_ANY(sbs_sbs_gauge);

	zassert_true(device_is_ready(dev), "Fuel Gauge not found");

	return dev;
}

void test_get_sensor_value(int16_t channel)
{
	/* Helper that only checks channel access success. */
	struct sensor_value value;
	const struct device *dev = get_fuel_gauge_device();

	zassert_ok(sensor_sample_fetch_chan(dev, channel), "Sample fetch failed");
	zassert_ok(sensor_channel_get(dev, channel, &value), "Get sensor value failed");
}

void test_get_sensor_value_not_supp(int16_t channel)
{
	const struct device *dev = get_fuel_gauge_device();

	zassert_true(sensor_sample_fetch_chan(dev, channel) == -ENOTSUP, "Invalid function");
}

void test_get_gauge_voltage(void)
{
	test_get_sensor_value(SENSOR_CHAN_GAUGE_VOLTAGE);
}

void test_get_gauge_current(void)
{
	test_get_sensor_value(SENSOR_CHAN_GAUGE_AVG_CURRENT);
}

void test_get_temperature(void)
{
	test_get_sensor_value(SENSOR_CHAN_GAUGE_TEMP);
}

void test_get_soc(void)
{
	test_get_sensor_value(SENSOR_CHAN_GAUGE_STATE_OF_CHARGE);
}

void test_get_full_charge_capacity(void)
{
	test_get_sensor_value(SENSOR_CHAN_GAUGE_FULL_CHARGE_CAPACITY);
}

void test_get_rem_charge_capacity(void)
{
	test_get_sensor_value(SENSOR_CHAN_GAUGE_REMAINING_CHARGE_CAPACITY);
}

void test_get_nom_avail_capacity(void)
{
	test_get_sensor_value(SENSOR_CHAN_GAUGE_NOM_AVAIL_CAPACITY);
}

void test_get_full_avail_capacity(void)
{
	test_get_sensor_value(SENSOR_CHAN_GAUGE_FULL_AVAIL_CAPACITY);
}

void test_get_average_time_to_empty(void)
{
	test_get_sensor_value(SENSOR_CHAN_GAUGE_TIME_TO_EMPTY);
}

void test_get_average_time_to_full(void)
{
	test_get_sensor_value(SENSOR_CHAN_GAUGE_TIME_TO_FULL);
}

void test_get_cycle_count(void)
{
	test_get_sensor_value(SENSOR_CHAN_GAUGE_CYCLE_COUNT);
}

void test_not_supported_channel(void)
{
	uint8_t channel;

	for (channel = SENSOR_CHAN_ACCEL_X; channel <= SENSOR_CHAN_RPM; channel++) {
		test_get_sensor_value_not_supp(channel);
	}
	/* SOH is not defined in the SBS 1.1 specifications */
	test_get_sensor_value_not_supp(SENSOR_CHAN_GAUGE_STATE_OF_HEALTH);

	/* These readings are not presently supported by the sbs_gauge driver. */
	test_get_sensor_value_not_supp(SENSOR_CHAN_GAUGE_STDBY_CURRENT);
	test_get_sensor_value_not_supp(SENSOR_CHAN_GAUGE_MAX_LOAD_CURRENT);
	test_get_sensor_value_not_supp(SENSOR_CHAN_GAUGE_DESIRED_VOLTAGE);
	test_get_sensor_value_not_supp(SENSOR_CHAN_GAUGE_DESIRED_CHARGING_CURRENT);
}
