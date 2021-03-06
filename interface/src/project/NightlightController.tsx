import React, { Component } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';
import { makeStyles } from '@material-ui/core/styles';
import Grid from '@material-ui/core/Grid';
import Paper from '@material-ui/core/Paper';
import TimePicker from './alarmclock/TimePicker'
import ColorPicker from 'material-ui-color-picker';
import Slider from '@material-ui/core/Slider';
import BrightnessLow from '@material-ui/icons/BrightnessLow';
import BrightnessHigh from '@material-ui/icons/BrightnessHigh';
import DateFnsAdapter from '@date-io/date-fns';


import { Typography, Box, Switch } from '@material-ui/core';
import { WEB_SOCKET_ROOT } from '../api';
import { WebSocketControllerProps, WebSocketFormLoader, WebSocketFormProps, webSocketController } from '../components';
import { SectionContent, BlockFormControlLabel } from '../components';

import { NightlightState } from './types';

export const LIGHT_SETTINGS_WEBSOCKET_URL = WEB_SOCKET_ROOT + "nightlightState";

const dateFns = new DateFnsAdapter();

type LightStateWebSocketControllerProps = WebSocketControllerProps<NightlightState>;

const useStyles = makeStyles((theme) => ({
  rightAligned: {
    textAlign: 'right',
  },
}));

class LightStateWebSocketController extends Component<LightStateWebSocketControllerProps> {

  render() {
    return (
      <SectionContent title='Nightlight' titleGutter>
        <WebSocketFormLoader
          {...this.props}
          render={props => (
            <LightStateWebSocketControllerForm {...props} />
          )}
        />
      </SectionContent>
    )
  }

}

export default webSocketController(LIGHT_SETTINGS_WEBSOCKET_URL, 100, LightStateWebSocketController);

type LightStateWebSocketControllerFormProps = WebSocketFormProps<NightlightState>;

function LightStateWebSocketControllerForm(props: LightStateWebSocketControllerFormProps) {
  const { data, saveData, setData } = props;


  const changeStatus = (event: React.ChangeEvent<{}>, status: number | number[]) => {
    if (Array.isArray(status)) {
      setData({ ...data, status: status[0] }, saveData);
    }
    else {
      setData({ ...data, status: status }, saveData);
    }
  }
  const changeLedColor = (color: String) => {
    let number = Number(`0x${color.slice(1)}`);
    let intVal = parseInt(`${number}`, 10);
    setData({ ...data, color: intVal }, saveData);
  }
  const changeBrightness = (event: React.ChangeEvent<{}>, brightness: number | number[]) => {
    if (Array.isArray(brightness)) {
      setData({ ...data, brightness: brightness[0] }, saveData);
    }
    else {
      setData({ ...data, brightness: brightness }, saveData);
    }
  }
  const changeStart = (start: number) => {
    setData({ ...data, start: start }, saveData);
  }
  const changeStop = (stop: number) => {
    setData({ ...data, stop: stop }, saveData);
  }
  const marks = [
    {
      value: 0,
      label: "Off"
    },
    {
      value: 1,
      label: "On"
    },
    {
      value: 2,
      label: "Test"
    }
  ]

  const classes = useStyles();



  return (
    <ValidatorForm onSubmit={saveData}>
      <Grid container spacing={3} >
        <Grid item xs={3}><Typography className={classes.rightAligned} variant="h6">Mode</Typography></Grid>
        <Grid item xs={6} >
          <Slider
            value={data.status}
            min={0}
            max={2}
            step={1}
            marks={marks}
            onChange={changeStatus}
            aria-labelledby="discrete-slider-custom"
          />
        </Grid>
        <Grid item xs={3}></Grid>

        <Grid item xs={3}><Typography className={classes.rightAligned} variant="h6">Color</Typography></Grid>
        <Grid item xs={6}>
          <ColorPicker
            name='color'
            defaultValue={"Nightlight Color"}
            value={`#${Number(data.color).toString(16)}`}
            onChange={changeLedColor}
            style={{ width: "100%" }}
          />
        </Grid>
        <Grid item xs={3}></Grid>
        <Grid item xs={3}><Typography className={classes.rightAligned} variant="h6">Brightness</Typography></Grid>
        <Grid item xs={6}>
          <Grid container spacing={2}>
            <Grid item>
              <BrightnessLow />
            </Grid>
            <Grid item xs>
              <Slider
                value={data.brightness}
                min={0}
                max={255}
                step={1}
                onChange={changeBrightness}
                aria-labelledby="continuous-slider"
              />
            </Grid>
            <Grid item>
              <BrightnessHigh />
            </Grid>
          </Grid>
        </Grid>
        <Grid item xs={3}></Grid>
        <Grid item xs={3}><Typography className={classes.rightAligned} variant="h6">Start Time</Typography></Grid>
        <Grid item xs={6}>
          <TimePicker
            onChange={changeStart}
            value={data.start}
          />
        </Grid>
        <Grid item xs={3}></Grid>
        <Grid item xs={3}><Typography className={classes.rightAligned} variant="h6">End Time</Typography></Grid>
        <Grid item xs={6}>
          <TimePicker
            onChange={changeStop}
            value={data.stop}
          />
        </Grid>
        <Grid item xs={3}></Grid>
      </Grid>
    </ValidatorForm>
  );
}
