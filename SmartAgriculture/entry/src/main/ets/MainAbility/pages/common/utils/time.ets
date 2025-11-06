/*
 * Copyright (c) 2022 JianGuo Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * @ProjectName : SmartAgriculture
 * @FileName : time
 * @Author : 坚果
 * @Time : 2022/12/5 17:30
 * @Description : 文件描述
 */
/*
 * Copyright (c) 2022 JianGuo Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * @ProjectName : CloudCommunity
 * @FileName : time
 * @Author : 坚果
 * @Time : 2022/10/10 15:15
 * @Description : 时间
 */
import featureAbility from '@ohos.ability.featureAbility';
import wantConstant from '@ohos.ability.wantConstant';

let date = new Date

export function getFullYear(): string {
  return date.getFullYear() + '年'
}

export function getDate(): string {
  return date.getMonth() + 1 + '月' + date.getDate() + '日'
}

export function getTime(): string {
  return (date.getHours() < 10 ? '0' + date.getHours() : date.getHours()) + ':' + (date.getMinutes() < 10 ? '0' + date.getMinutes() : date.getMinutes())
}

export function getSeconds(): string {
  return ':' + (date.getSeconds() < 10 ? '0' + date.getSeconds() : date.getSeconds())
}

export function toTime(Number: number): string {
  let hour: number
  let minute: number
  let second: number
  hour = Number / 3600 | 0
  minute = Number / 60 % 60 | 0
  second = Number % 60 | 0
  if (hour > 0) {
    return (hour < 10 ? '0' + hour : hour.toString()) + ":" + (minute < 10 ? '0' + minute : minute.toString()) + ":" + (second < 10 ? '0' + second : second.toString())
  } else {
    return (minute < 10 ? '0' + minute : minute.toString()) + ":" + (second < 10 ? '0' + second : second.toString())
  }
}

/**
 * 拉起远程设备
 */
export function startRemoteAbilities(deviceIds, videosId) {
  for (var i = 0; i < deviceIds.length; i++) {
    var want = {
      "want": {
        "deviceId": deviceIds[i],
        "bundleName": "com.jianguo.myapplication",
        "abilityName": "com.jianguo.myapplication.MainAbility",
        // 分布式任务flag
        "flags": wantConstant.Flags.FLAG_ABILITYSLICE_MULTI_DEVICE,
        "parameters": {
          // 指定跳转的页面
          "url": 'pages/index',
          // 跳转携带的参数
          "videosId": videosId
        },
      }
    };
    featureAbility.startAbility(want, (err, data) => {
      if (err) {
        console.error('startRemoteAbility err:' + JSON.stringify(err));
        return;
      }
      console.info('startRemoteAbility successful. Data: ' + JSON.stringify(data))
    });
  }
}