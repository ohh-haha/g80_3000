/* Copyright 2019 ftfunjth
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "quantum.h"

/* This a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 */
#define LAYOUT( \
      k1c1,    k1c2,    k1c3  ,  k1c4  ,  k1c5  ,  k1c6           ,  k1c8            ,  k1c10            ,  k1c12  ,  k1c13  ,            k1r11  ,                              \
      k2c1,    k2c2,    k2c3  ,  k2c4           ,  k2c6  ,  k2c7  ,  k2c8  ,  k2c9   ,  k2c10  ,  k2c11            ,  k2c13  ,                      k2r12  ,                    \
      k3c1,    k3c2,    k3c3  ,  k3c4           ,  k3c6  ,  k3c7  ,  k3c8  ,  k3c9   ,  k3c10  ,  k3c11  ,  k3c12  ,  k3c13  ,                      k3r12            ,  k3r14,  \
      k4c1,    k4c2,    k4c3  ,  k4c4  ,  k4c5           ,  k4c7  ,  k4c8  ,  k4c9   ,  k4c10  ,  k4c11  ,  k4c12  ,  k4c13  ,                      k4r12  ,                    \
      k5c1,                                                                                                                                                                     \
      k6c1,    k6c2,    k6c3  ,  k6c4           ,  k6c6  ,  k6c7  ,  k6c8  ,  k6c9   ,  k6c10  ,  k6c11  ,  k6c12  ,  k6c13  ,                      k6r12  ,  k6r13  ,          \
      k7c1,    k7c2,    k7c3  ,  k7c4  ,  k7c5  ,  k7c6  ,  k7c7  ,  k7c8  ,  k7c9   ,  k7c10  ,  k7c11  ,  k7c12  ,  k7c13  ,                      k7r12  ,                    \
               k8c2,    k8c3  ,  k8c4           ,  k8c6  ,  k8c7  ,           k8c9   ,  k8c10  ,  k8c11  ,  k8c12  ,  k8c13  ,                      k8r12  ,  k8r13  ,  k8r14,  \
      k9c1,    k9c2           ,  k9c4  ,  k9c5  ,  k9c6  ,  k9c7  ,  k9c8            ,  k9c10  ,  k9c11  ,  k9c12  ,  k9c13  ,  k9c14                                           \
      ) { \
    { k1c1  ,  k1c2  ,  k1c3  ,  k1c4  ,  k1c5  ,  k1c6  ,  KC_NO ,  k1c8  ,  KC_NO  ,  k1c10  ,  KC_NO  ,  k1c12  ,  k1c13  ,  KC_NO  ,  k1r11  ,  KC_NO  ,  KC_NO  ,  KC_NO}, \
    { k2c1  ,  k2c2  ,  k2c3  ,  k2c4  ,  KC_NO ,  k2c6  ,  k2c7  ,  k2c8  ,  k2c9   ,  k2c10  ,  k2c11  ,  KC_NO  ,  k2c13  ,  KC_NO  ,  KC_NO  ,  k2r12  ,  KC_NO  ,  KC_NO}, \
    { k3c1  ,  k3c2  ,  k3c3  ,  k3c4  ,  KC_NO ,  k3c6  ,  k3c7  ,  k3c8  ,  k3c9   ,  k3c10  ,  k3c11  ,  k3c12  ,  k3c13  ,  KC_NO  ,  KC_NO  ,  k3r12  ,  KC_NO  ,  k3r14}, \
    { k4c1  ,  k4c2  ,  k4c3  ,  k4c4  ,  k4c5  ,  KC_NO ,  k4c7  ,  k4c8  ,  k4c9   ,  k4c10  ,  k4c11  ,  k4c12  ,  k4c13  ,  KC_NO  ,  KC_NO  ,  k4r12  ,  KC_NO  ,  KC_NO}, \
    { k5c1  ,  KC_NO ,  KC_NO ,  KC_NO ,  KC_NO ,  KC_NO ,  KC_NO ,  KC_NO ,  KC_NO  ,  KC_NO  ,  KC_NO  ,  KC_NO  ,  KC_NO  ,  KC_NO  ,  KC_NO  ,  KC_NO  ,  KC_NO  ,  KC_NO}, \
    { k6c1  ,  k6c2  ,  k6c3  ,  k6c4  ,  KC_NO ,  k6c6  ,  k6c7  ,  k6c8  ,  k6c9   ,  k6c10  ,  k6c11  ,  k6c12  ,  k6c13  ,  KC_NO  ,  KC_NO  ,  k6r12  ,  k6r13  ,  KC_NO}, \
    { k7c1  ,  k7c2  ,  k7c3  ,  k7c4  ,  k7c5  ,  k7c6  ,  k7c7  ,  k7c8  ,  k7c9   ,  k7c10  ,  k7c11  ,  k7c12  ,  k7c13  ,  KC_NO  ,  KC_NO  ,  k7r12  ,  KC_NO  ,  KC_NO}, \
    { KC_NO ,  k8c2  ,  k8c3  ,  KC_NO ,  KC_NO ,  k8c6  ,  k8c7  ,  KC_NO ,  k8c9   ,  k8c10  ,  k8c11  ,  k8c12  ,  k8c13  ,  KC_NO  ,  KC_NO  ,  k8r12  ,  k8r13  ,  k8r14}, \
    { k9c1  ,  k9c2  ,  KC_NO ,  k9c4  ,  k9c5  ,  k9c6  ,  k9c7  ,  k9c8  ,  KC_NO  ,  k9c10  ,  k9c11  ,  k9c12  ,  k9c13  ,  k9c14  ,  KC_NO  ,  KC_NO  ,  KC_NO  ,  KC_NO}, \
}
