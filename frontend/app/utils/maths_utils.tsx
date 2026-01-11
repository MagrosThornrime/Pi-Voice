export function getADSRFactor(t: number, minTime: number, maxTime: number){
    const sampleRate = 44100;
    const time = minTime * (maxTime / minTime) ** t;
    return 1 / (time * sampleRate);
}

export function calcLogValueFromScale(x:number, lims:number[]){
    // changes position on slider to log10 value of number, which bound are in lims
    let res = Math.log10( lims[0] ) + x/100 * ( Math.log10(lims[1]) - Math.log10(lims[0]) )
    return res;
}


export function calcLogaritmicPosFromLinear(x:number, linLims:number[], logLims:number[]){
    // changes position on slider for linear scale into logarithmic scale
    const actVal = linLims[0] + (linLims[1] - linLims[0]) * x/100
    const logMin = Math.log10(logLims[0]);
    const logMax = Math.log10(logLims[1]);
    const logVal = Math.log10(actVal);

    const logPos = (logVal - logMin) / (logMax - logMin);

    return Math.round(logPos * 100);
}

export function calcLinearPosFromLogarithmic(x:number, logLims:number[], linLims:number[]){
    const actVal = Math.pow(10, calcLogValueFromScale(x, logLims))
    return Math.round((actVal - linLims[0])/(linLims[1] - linLims[0]) * 100);
}


export function getBounds(x:number, lims: number[]):number[] {
    let lower_bound = Math.pow(10, Math.floor(Math.log10(x)))
    let upper_bound = Math.pow(10, Math.ceil(Math.log10(x)))

    if (lower_bound === upper_bound){
        if (lower_bound === lims[1]){
            return [lower_bound/10, lower_bound]
        }
        return [lower_bound, lower_bound*10]
    }
    if (upper_bound > lims[1]){
        upper_bound = lims[1]
    }
    if (lower_bound < lims[0]){
        lower_bound = lims[0];
    }

    return [lower_bound, upper_bound]
}


export function calcValueFromLogScale(x:number, lims: number[]){
    return Math.pow(10, calcLogValueFromScale(x, lims));
}


export function calcValueFromLinScale(x:number, lims:number[]){
    return lims[0] + x/100 * (lims[1] - lims[0]);
}


export type Point = {
  x: number;
  y: number;
}


export function get_example_data(
  n: number,
  domain: number[],
  func: (x: number) => number
): Point[] {
  return Array.from({ length: n }, (_, i): Point => {
    const x = domain[0] + (i * (domain[1] - domain[0])) / n;
    return {
      x,
      y: func(x),
    };
  });
}


export function square_wave(x: number, interv: number) {
  let position = x % interv;
  let eps = 0.01;

  if (position < eps) {
    return 0;
  }

  if (Math.abs(position - interv / 2) < eps) {
    if (position > interv / 2) {
      return 1;
    }
    return 0;
  }

  if (position < interv / 2) {
    return 1;
  }

  return 0;
}


export function triangle_wave(x: number, interv: number) {
  let position = x % interv;
  console.log("position:", position)

  if (position <= interv / 2) {
    return position;
  }
  return interv / 2 - (position - interv / 2);
}


export function sawtooth_func(x: number, interv: number) {
  let position = x % interv;
  let eps = 0.01;
  if (x < eps) {
    return 0.0;
  }
  if (position < eps) {
    return 1 / 2 * interv;
  }
  if (interv - position < eps) {
    return 0.0;
  }
  return 1 / 2 * position;

}


export const oscillatorsFuncMapping: Record<string, (X: number) => number> = {
  sine: ((x) => Math.sin(3 * x)),
  square: ((x => square_wave(x, 2.0))),
  triangle: ((x) => triangle_wave(x, 2.0)),
  empty: (() => 0.0),
  sawtooth: ((x) => sawtooth_func(x, 2.0))
}


export function getOscillatorFunction(name: string) {
  return oscillatorsFuncMapping[name] ?? oscillatorsFuncMapping["empty"]
}