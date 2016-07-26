/*
	This file is part of cpp-elementrem.
	cpp-elementrem is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	cpp-elementrem is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with cpp-elementrem.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file TabStyle.qml
 * @
 * 
 * 
 * Elementrem IDE client.
 */

import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

TabViewStyle {
	frameOverlap: 1
	tabBar: Rectangle {
		color: "lightgray"
	}
	tab: Rectangle {
		color: "lightsteelblue"
		implicitWidth: Math.max(text.width + 4, 80)
		implicitHeight: 20
		radius: 2
		DefaultText {
			id: text
			anchors.centerIn: parent
			text: styleData.title
			color: styleData.selected ? "white" : "black"
		}
	}
	frame: Rectangle { color: "steelblue" }
}