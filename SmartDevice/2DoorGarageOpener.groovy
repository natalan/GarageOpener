/**
 *  Two Doors Garage Opener
 *
 *  Author: Andrei Zharau
 *  Date: 01-19-2014
 */
 
metadata {
	// tile definitions
	tiles {
		standardTile("switch1", "device.switch1", width: 2, height: 2, canChangeIcon: true, canChangeBackground: true) {
			state "open1", label: 'push', action: "close1", icon: "st.Transportation.transportation2", backgroundColor: "#79b821"
			state "close1", label: 'push', action: "open1", icon: "st.Transportation.transportation2", backgroundColor: "#ffffff"
		}

		standardTile("switch2", "device.switch2", width: 2, height: 2, canChangeIcon: true, canChangeBackground: true) {
			state "open2", label: 'push', action: "close2", icon: "st.Transportation.transportation8", backgroundColor: "#79b821"
            state "close2", label: 'push', action: "open2", icon: "st.Transportation.transportation8", backgroundColor: "#ffffff"
		}
        
		main "switch1"
		details(["switch1", "switch2"])
	}
}

Map parse(String description) {
	def value = zigbee.parse(description)?.text

	log.debug "Received callback: " + description
	log.debug "Parsed value: " + value

	def linkText = getLinkText(device)
	def descriptionText = getDescriptionText(description, linkText, value)
	def handlerName = value
	def isStateChange = value != "ping"
	def displayed = value && isStateChange
	def device = null
	// find switch that called the cloud
	if (value in ["open1","close1"]) {
	    device = "switch1"
	} else if (value in ["open2","close2"]) {
	    device = "switch2"
	}

	def result = [
		value: value,
		name: device,
		handlerName: handlerName,
		linkText: linkText,
		descriptionText: descriptionText,
		isStateChange: isStateChange,
		displayed: displayed
	]

	log.debug "Message received from the garage opener: " + result
	result
}

def open1() {
    log.debug "opening first door"
    zigbee.smartShield(text: "open1").format()
}

def close1() {
    log.debug "closing first door"
	zigbee.smartShield(text: "close1").format()
}

def open2() {
    log.debug "opening second door"
    zigbee.smartShield(text: "open2").format()
}

def close2() {
    log.debug "closing second door"
	zigbee.smartShield(text: "close2").format()
}