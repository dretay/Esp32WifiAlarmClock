export interface AlarmSettings {
  entries: Array<{
    song: number,
    time: number,
    daysOfWeek: string,
    enabled: boolean,
  }>
}
export interface NightlightState {
  status: number;
  color: number;
  brightness: number;
  start: number;
  stop: number;
}

export interface LightState {
  led_on: boolean;
}

export interface LightMqttSettings {
  unique_id: string;
  name: string;
  mqtt_path: string;
}
