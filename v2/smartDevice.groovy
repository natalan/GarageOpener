/**
 *  Arduino
 *
 *  Copyright 2014 Daniel Ogorchock
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
 *  in compliance with the License. You may obtain a copy of the License at:
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software distributed under the License is distributed
 *  on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License
 *  for the specific language governing permissions and limitations under the License.
 *
 *
 *
 *  Based on
 *  	Garage Door
 *  	Original Author: Steve Sell
 *  	Date: 2014-02-02
 *
 *  Modified and Extended for my personal use
 * 	Author: Dan Ogorchock
 *      Date: 2014-07-05
 *
 *  Updated for my system
 *  Author: Andrei Zharov, 01/17/2015
 */

metadata {
    // Automatically generated. Make future change here.
    definition (name: "Garage Door Opener v2", namespace: "belmass", author: "Andrei Zharov") {
        capability "Polling"
        capability "Refresh"

        attribute "leftDoor", "string"
        attribute "rightDoor", "string"
        attribute "officeDoor", "string"
        attribute "garageDoor", "string"
        attribute "laundryDoor", "string"
        attribute "gymDoor", "string"

        command "pushLeft"
        command "pushRight"
    }

    simulator {
        status "on":  "catchall: 0104 0000 01 01 0040 00 0A21 00 00 0000 0A 00 0A6F6E"
        status "off": "catchall: 0104 0000 01 01 0040 00 0A21 00 00 0000 0A 00 0A6F6666"

        // reply messages
        reply "raw 0x0 { 00 00 0a 0a 6f 6e }": "catchall: 0104 0000 01 01 0040 00 0A21 00 00 0000 0A 00 0A6F6E"
        reply "raw 0x0 { 00 00 0a 0a 6f 66 66 }": "catchall: 0104 0000 01 01 0040 00 0A21 00 00 0000 0A 00 0A6F6666"
    }

    // Preferences


    // tile definitions
    tiles {
        standardTile("leftDoor", "device.leftDoor", width: 1, height: 1, canChangeIcon: true, canChangeBackground: true) {
            state "closed", label: 'Closed', action: "pushLeft", icon: "st.doors.garage.garage-closed", backgroundColor: "#79b821", nextState: "opening"
            state "open", label: 'Open', action: "pushLeft", icon: "st.doors.garage.garage-open", backgroundColor: "#ffa81e", nextState: "closing"
            state "opening", label: 'Opening', action: "pushLeft", icon: "st.doors.garage.garage-opening", backgroundColor: "89C2E8", nextState: "opening"
            state "closing", label: 'Closing', action: "pushLeft", icon: "st.doors.garage.garage-closing", backgroundColor: "89C2E8", nextState: "closing"
        }
        standardTile("rightDoor", "device.rightDoor", width: 1, height: 1, canChangeIcon: true, canChangeBackground: true) {
            state "closed", label: 'Closed', action: "pushRight", icon: "st.doors.garage.garage-closed", backgroundColor: "#79b821", nextState: "opening"
            state "open", label: 'Open', action: "pushRight", icon: "st.doors.garage.garage-open", backgroundColor: "#ffa81e", nextState: "closing"
            state "opening", label: 'Opening', action: "pushRight", icon: "st.doors.garage.garage-opening", backgroundColor: "89C2E8", nextState: "opening"
            state "closing", label: 'Closing', action: "pushRight", icon: "st.doors.garage.garage-closing", backgroundColor: "89C2E8", nextState: "closing"
        }
        standardTile("officeDoor", "device.officeDoor", width: 1, height: 1, canChangeIcon: true, canChangeBackground: true) {
            state("open", label:'${name}', icon:"st.contact.contact.open", backgroundColor:"#ffa81e")
            state("closed", label:'${name}', icon:"st.contact.contact.closed", backgroundColor:"#79b821")
        }
        standardTile("garageDoor", "device.garageDoor", width: 1, height: 1, canChangeIcon: true, canChangeBackground: true) {
            state("open", label:'${name}', icon:"st.contact.contact.open", backgroundColor:"#ffa81e")
            state("closed", label:'${name}', icon:"st.contact.contact.closed", backgroundColor:"#79b821")
        }
        standardTile("laundryDoor", "device.laundryDoor", width: 1, height: 1, canChangeIcon: true, canChangeBackground: true) {
            state("open", label:'${name}', icon:"st.contact.contact.open", backgroundColor:"#ffa81e")
            state("closed", label:'${name}', icon:"st.contact.contact.closed", backgroundColor:"#79b821")
        }
        standardTile("gymDoor", "device.gymDoor", width: 1, height: 1, canChangeIcon: true, canChangeBackground: true) {
            state("open", label:'${name}', icon:"st.contact.contact.open", backgroundColor:"#ffa81e")
            state("closed", label:'${name}', icon:"st.contact.contact.closed", backgroundColor:"#79b821")
        }

        main "leftDoor"
        details(["leftDoor","rightDoor","officeDoor","garageDoor","laundryDoor","gymDoor"])
    }
}

//Map parse(String description) {
def parse(String description) {
    def name = null
    def value = zigbee.parse(description)?.text
    log.debug "Value is ${value}"
    def linkText = getLinkText(device)
    def descriptionText = getDescriptionText(description, linkText, value)
    def handlerName = value
    def isStateChange = value != "ping"
    def displayed = value && isStateChange

    def incoming_cmd = value.split()

    name = incoming_cmd[0]
    value = incoming_cmd[1]

    def result = [
            value: value,
            name: value != "ping" ? name : null,
            handlerName: handlerName,
            linkText: linkText,
            descriptionText: descriptionText,
            isStateChange: isStateChange,
            displayed: displayed
    ]
    log.debug result
    return result

}

def pushLeft() {
    log.debug "Executing 'pushLeft'"
    zigbee.smartShield(text: "pushLeft").format()
}

def pushRight() {
    log.debug "Executing 'pushRight'"
    zigbee.smartShield(text: "pushRight").format()
}
