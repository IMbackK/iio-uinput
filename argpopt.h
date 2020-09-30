/**
 * Sigstoped
 * Copyright (C) 2020 Carl Klemm
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 3 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 */

#pragma once
#include<argp.h>
#include<string>

struct Config
{
    int rate = -1;
	std::string device;
};

const char *argp_program_version = "1.0";
const char *argp_program_bug_address = "<carl@uvos.xyz>";
static char doc[] = "Tool to allow the use of iio acclerometers as evdev input devices";
static char args_doc[] = "";

static struct argp_option options[] = 
{
  {"rate",  'r', "rate",      0,  "Rate in Hz to run the acclerometer at" },
  {"device", 'd', "iio device",      0,  "Set the iio device you want to use here" },
  { 0 }
};


error_t parse_opt (int key, char *arg, struct argp_state *state)
{
    Config* config = reinterpret_cast<Config*>(state->input);
    switch (key)
    {
        case 'r':
        config->rate = atol(arg);
        break;
        case 'd':
        config->device.assign(arg);
        break;
        default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };


