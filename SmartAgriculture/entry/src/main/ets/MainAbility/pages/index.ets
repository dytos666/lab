import router from '@ohos.router';
import prompt from '@ohos.prompt';
import { getDate } from './common/utils/time';
import { getFullYear } from './common/utils/time';
import { getSeconds } from './common/utils/time';
import { getTime } from './common/utils/time';
import { TextRow } from './common/optionItem';
import { ClickText } from './common/optionItem';

import wifi from '@ohos.wifi';
import socket from '@ohos.net.socket';

//tcp连接对象
let tcp = socket.constructTCPSocketInstance();
//目标地址和端口
let targetAddr = {
  address: '192.168.84.58', //要通信的地址，3861
  family: 1,
  port: 3861 //
}

enum MSG_CMD {
  Switch1_ON= "{\"switch1\":\"on\", }", //移动


  Switch1_OFF= "{\"switch2\":\"off\", }", //移动
  Switch2_ON = "{\"switch2\":\"on\", }", //移动
  Switch2_OFF= "{\"switch2\":\"off\", }", //移动

};
@Entry
@Component
struct Index {
  @State message: string = 'Hello World'
  private tabSrc: Resource[] = [$r("app.media.zhny1"), $r("app.media.zhny2"), $r("app.media.zhny3"), $r("app.media.zhny4")]
  private swiperController: SwiperController = new SwiperController()
  @State currentDate: string = getFullYear() + getDate()
  @State currentTime: string = getTime() + getSeconds()
  @State temp: string = "22.60"
  @State humi: string = "32.82"
  //  @State mc1: string = "1"
  //  @State mc2: string = "1"
  //  @State rt1: string = "1"
  @State isNight: boolean = false
  quality: string = '优'
  @State message_send: string = '{"temp":32.27,"humi":32.27,"rt1":1,"mc1":1,"mc2":1}'
  @State message_recv: string = ''
  @State status: string = '未连接'

  private attendanceOnClick1ON=(event: ClickEvent)=> {
    this.message_send = MSG_CMD.Switch1_ON
    this.tcpSend()
  }

  private attendanceOnClick1Off=(event: ClickEvent)=> {
    this.message_send = MSG_CMD.Switch1_OFF
    this.tcpSend()
  }

  private attendanceOnClick2ON=(event: ClickEvent)=> {
    this.message_send = MSG_CMD.Switch2_ON
    this.tcpSend()
  }

  private attendanceOnClick2Off=(event: ClickEvent)=> {
    this.message_send = MSG_CMD.Switch2_OFF
    this.tcpSend()
  }

  onPageShow() {
    //    this.tcpConnect()
    setInterval(() => {
      let date = new Date
      // 判断是否为夜晚
      this.isNight = date.getHours() >= 22 ? true : false
      // 获取当前时间
      this.currentTime = (date.getHours() < 10 ? '0' + date.getHours() : date.getHours()) + ':' + (date.getMinutes() < 10 ? '0' + date.getMinutes() : date.getMinutes()) + ':' + (date.getSeconds() < 10 ? '0' + date.getSeconds() : date.getSeconds())
      // 判断是否为新的一天
      if (date.getHours() == 0) {
        this.currentDate = date.getFullYear() + '年' + (date.getMonth() + 1) + '月' + date.getDate() + '日'
      }
    }, 1000)
  }

  aboutToAppear() {
    this.tcpConnect()
  }

  aboutToDisappear() {
    this.tcpClose()
  }
  //本地地址和端口
  localAddr = {
    address: this.resolveIP(wifi.getIpInfo().ipAddress)
  }

  //解析本地IP,需要声明ohos.permission.GET_WIFI_INFO权限
  resolveIP(ip) {
    if (ip < 0 || ip > 0xFFFFFFFF) {
      throw ("The number is not normal!");
    }
    return (ip >>> 24) + "." + (ip >> 16 & 0xFF) + "." + (ip >> 8 & 0xFF) + "." + (ip & 0xFF);
  }
  //解析ArrayBuffer
  resolveArrayBuffer(message: ArrayBuffer): string {
    if (message instanceof ArrayBuffer) {
      let dataView = new DataView(message)
      console.info(`length ${dataView.byteLength}`)
      let str = ""
      for (let i = 0;i < dataView.byteLength; ++i) {
        let c = String.fromCharCode(dataView.getUint8(i))
        if (c !== "\n") {
          str += c
        }
      }
      console.info(`message aray buffer:${str}`)
      return str;
    }
  }
  //初始化
  tcpInit() {
    tcp.on('connect', () => {
      this.status = '已连接'
      console.info("on tcp connect success");
    });
    tcp.on('message', value => {
      this.message_recv = this.resolveArrayBuffer(value.message)
      var temp = "temp";
      var humi = "humi";
      if (this.message_recv.includes(temp)) {

        this.temp = this.message_recv.slice(5, 10);
        console.log("==收到消息温度" + this.message_recv.slice(5, 10))
      }
      if (this.message_recv.includes(humi)) {

        this.humi = this.message_recv.slice(5, 10);
        console.log("==收到消息湿度" + this.message_recv.slice(5, 10))
      }
      if (this.message_recv.includes("mc1")) {
        let mc1 = this.message_recv.slice(4, 5);

        //        this.mc1 = this.message_recv.slice(4, 5);
        console.log("==收到消息门禁1" + this.message_recv.slice(4, 5))
      }
      if (this.message_recv.includes("mc2")) {

        //        this.mc2 = this.message_recv.slice(4, 5);
        console.log("==收到消息门禁2" + this.message_recv.slice(4, 5))
      }
      if (this.message_recv.includes("rt1")) {

        //        this.rt1 = this.message_recv.slice(4, 5);
        console.log("==收到消息有人经过" + this.message_recv.slice(4, 5))
      }

      console.log("==收到消息:" + this.message_recv)


    });
    tcp.on('close', () => {
      console.info(" ==收到 on tcp close success");
    });

    //bind本地地址
    tcp.bind({ address: this.localAddr.address, port: 3861, family: 1 })
      .then(() => {
        console.info(` ==收到 bind tcp success`);
      }).catch(err => {
      console.info(` ==收到 bind tcp failed ${err}`);
      return
    });
  }
  //tcp 连接
  tcpConnect() {
    tcp.getState()
      .then((data) => {
        console.info(`====${JSON.stringify(data)}`)
        if (data.isClose) {
          this.tcpInit()
        }

        //开始连接
        tcp.connect(
          {
            address: { address: targetAddr.address, port: 3861, family: 1 }, timeout: 6000
          }
        ).then(() => {
          console.info(` ==收到 connect success`);

        }).catch((error) => {
          console.info(` ==收到 connect failed ${JSON.stringify(error)}`);
        })

      })
  }

  tcpSend() {
    //查看状态
    tcp.getState().then((data) => {
      console.info(`====${JSON.stringify(data)}`)
      //已连接,就发送数据
      if (data.isConnected) {
        //发送消息
        tcp.send(
          { data: this.message_send, }
        ).then(() => {
          prompt.showToast({ message: "数据发送成功" + this.message_send })
        }).catch((error) => {
          prompt.showToast({ message: `send failed ${JSON.stringify(error)}` })

        })
      } else {
        prompt.showToast({ message: "not connect" })

      }
    })
  }

  tcpClose() {
    tcp.close().then(() => {
      this.status = '已断开'
      console.info(`tcp.close success`)
    }).catch((err) => {
      console.info(`tcp.close error:${JSON.stringify(err)}`)
    })
    tcp.off('close');
    tcp.off('message');
    tcp.off('connect');
  }

  build() {
    Row() {


      Column() {
        //温度，湿度，光照强度，土壤湿度，
        Text("智慧农业系统")
          .fontSize(36)
          .fontWeight(FontWeight.Bold)
        //当地温度
        Column() {
          Row() {
            if (this.isNight) {
              Image($r('app.media.moon'))
                .width(100)
                .height(100)
                .objectFit(ImageFit.None)
            } else {
              Image($r('app.media.sun'))
                .width(100)
                .height(100)
                .objectFit(ImageFit.None)
            }
            Text(this.currentDate + '\n' + this.currentTime)
              .fontSize(24)
              .fontColor(Color.White)
          }

          TextRow({ text: '温度', value: this.temp + '℃' })
          TextRow({ text: '空气质量', value: this.quality })
          TextRow({ text: '空气湿度', value: this.humi + "%RH" })
        }
        .onAppear(() => {
          //          this.tcpConnect()
        })
        .onClick(() => {
          //          this.tcpConnect()
        })
        .width("90%")
        .borderRadius(30)
        .backgroundColor($r('app.color.blue_link'))
        .margin(20)
        //视频监控
        Swiper(this.swiperController) {
          ForEach(this.tabSrc, (item) => {
            Image(item).width('90%').height("30%").backgroundColor(0xAFEEEE)
          },)
        }
        .cachedCount(2)
        .index(1)
        .autoPlay(true)
        .interval(4000)
        .indicator(true) // 默认开启指示点
        .loop(true) // 默认开启循环播放
        .duration(1000)
        .vertical(false) // 默认横向切换
        .itemSpace(0)
        .curve(Curve.Linear) // 动画曲线
        .onChange((index: number) => {
          console.info(index.toString())
        })
        .margin(10)
        //控制区域
        Flex({ wrap: FlexWrap.Wrap, direction: FlexDirection.Row }) {
          ClickText({ context: "通风", itemOnClick: this.attendanceOnClick1ON })
          ClickText({ context: "浇水", itemOnClick: this.attendanceOnClick1Off })
          ClickText({ context: "施肥", itemOnClick: this.attendanceOnClick2ON })
          ClickText({ context: "补光", width: "100%", itemOnClick: this.attendanceOnClick2Off })
        }
        //控制区域，通风，浇水，补光，施肥
        //        this.message_send = MSG_CMD.MOVE_GO
        //        this.tcpSend()

      }
      .width('100%')
    }
    .height('100%')
  }
}