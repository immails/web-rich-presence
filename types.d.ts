declare namespace WebRichPresence {
	export enum WSCommand {
		SetApplicationID, Clear, SetType, SetDetails, SetState, SetName, SetTimestamps, SetAssets
	}

	export enum ActivityTypes {
		Playing = 0,
		Streaming = 1,
		Listening = 2,
		Watching = 3,
		CustomStatus = 4,
		Competing = 5,
		HangStatus = 6,
	}

	export interface Command {
		"command": WSCommand
	}

	namespace Commands {
		export interface SetApplicationID {
			"command": WSCommand.SetApplicationID,
			"id": string
		}

		export interface Clear {
			"command": WSCommand.Clear
		}

		export interface SetType {
			"command": WSCommand.SetType,
			"type": ActivityTypes
		}

		export interface SetDetails {
			"command": WSCommand.SetDetails,
			"details": string | null
		}

		export interface SetState {
			"command": WSCommand.SetState,
			"state": string | null
		}

		export interface SetName {
			"command": WSCommand.SetName,
			"name": string
		}

		export interface SetTimestamps {
			"command": WSCommand.SetTimestamps,
			"start"?: number | null,
			"end"?: number | null
		}

		export interface SetAssets {
			"command": WSCommand.SetAssets,
			"large_image_key"?: string | null
			"large_image_text"?: string | null
			"large_image_url"?: string | null
			"small_image_key"?: string | null
			"small_image_text"?: string | null
			"small_image_url"?: string | null
			// TODO: Add other assets
		}
	}
}