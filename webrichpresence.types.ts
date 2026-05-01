export enum WS_SERVER_COMMANDS {
	SetApplicationID, Clear, SetType, SetDetails, SetState, SetName, SetTimestamps, SetAssets, 
};

export enum WS_CLIENT_COMMANDS {
	SendVersion
};

export enum ACTIVITY_TYPES {
	Playing = 0,
	Streaming = 1,
	Listening = 2,
	Watching = 3,
	CustomStatus = 4,
	Competing = 5,
	HangStatus = 6,
}

export namespace ServerCommands {
	export interface BaseServerCommand {
		"command": WS_SERVER_COMMANDS
	}

	export interface SetApplicationID extends BaseServerCommand {
		"command": WS_SERVER_COMMANDS.SetApplicationID,
		"id": string
	}

	export interface Clear extends BaseServerCommand {
		"command": WS_SERVER_COMMANDS.Clear
	}

	export interface SetType extends BaseServerCommand {
		"command": WS_SERVER_COMMANDS.SetType,
		"type": ACTIVITY_TYPES
	}

	export interface SetDetails extends BaseServerCommand {
		"command": WS_SERVER_COMMANDS.SetDetails,
		"details": string | null
	}

	export interface SetState extends BaseServerCommand {
		"command": WS_SERVER_COMMANDS.SetState,
		"state": string | null
	}

	export interface SetName extends BaseServerCommand {
		"command": WS_SERVER_COMMANDS.SetName,
		"name": string
	}

	export interface SetTimestamps extends BaseServerCommand {
		"command": WS_SERVER_COMMANDS.SetTimestamps,
		"start"?: number | null,
		"end"?: number | null
	}

	export interface SetAssets extends BaseServerCommand {
		"command": WS_SERVER_COMMANDS.SetAssets,
		"large_image_key"?: string | null
		"large_image_text"?: string | null
		"large_image_url"?: string | null
		"small_image_key"?: string | null
		"small_image_text"?: string | null
		"small_image_url"?: string | null
		// TODO: Add other assets
	}
}

export namespace ClientCommands {
	export interface BaseClientCommand {
		"command": WS_CLIENT_COMMANDS
	}

	export interface SendVersion extends BaseClientCommand {
		"command": WS_CLIENT_COMMANDS.SendVersion,
		"version": string
	}
}