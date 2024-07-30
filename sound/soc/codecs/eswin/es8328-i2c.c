// SPDX-License-Identifier: GPL-2.0
/*
 * ESWIN Codec root complex driver
 *
 * Copyright 2024, Beijing ESWIN Computing Technology Co., Ltd.. All rights reserved.
 * SPDX-License-Identifier: GPL-2.0
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Authors: Lei Deng <denglei@eswincomputing.com>
 */

#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/regmap.h>

#include <sound/soc.h>

#include "es8328.h"

static const struct i2c_device_id es8328_id[] = {
	{ "es8328", 0 },
	{ "es8388", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, es8328_id);

static const struct of_device_id es8328_of_match[] = {
	{ .compatible = "eswin,es8388", },
	{ }
};
MODULE_DEVICE_TABLE(of, es8328_of_match);

static int es8328_i2c_probe(struct i2c_client *i2c)
{
	dev_info(&i2c->dev, "dev name:%s\n", i2c->dev.of_node->name);
	return es8328_probe(&i2c->dev,
			devm_regmap_init_i2c(i2c, &es8328_regmap_config));
}

static struct i2c_driver es8328_i2c_driver = {
	.driver = {
		.name		= "es8328",
		.of_match_table = es8328_of_match,
	},
	.probe    = es8328_i2c_probe,
	.id_table = es8328_id,
};

module_i2c_driver(es8328_i2c_driver);

MODULE_DESCRIPTION("ASoC ES8328 audio CODEC I2C driver");
MODULE_AUTHOR("Sean Cross <xobs@kosagi.com>");
MODULE_LICENSE("GPL");
