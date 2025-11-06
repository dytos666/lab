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
 * @FileName : optionItem
 * @Author : 坚果
 * @Time : 2022/12/5 16:38
 * @Description :
 */


/*
 * Copyright (c) 2021 JianGuo Device Co., Ltd.
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
 * 该组件为设置和其他类似UI的选项布局。
 * 主要内容为图片+文字+导航箭头

 */



@Component
export struct optionItem {
  @State bgColor: Resource = $r("app.color.white")
  private img: Resource //图标资源
  private context: string //文字内容
  private isNext: boolean //是否有下一步的导航箭头
  private marginTop: string = "100%" //组件上边距

  private itemOnClick = (event: ClickEvent) => {
  } //item的单击事件（可选）

  build() {
    Row() {
      if (this.img != null) {
        Flex({ direction: FlexDirection.Row, alignItems: ItemAlign.Center, justifyContent: FlexAlign.Center }) {
          Image(this.img)
            .width(30).height(30)
        }.width(50).height(60)
      }
      Column() {
        Row() {
          Text(this.context)
            .fontFamily($r('sys.float.ohos_id_text_size_body1'))
            .fontSize(19)
            .textOverflow({ overflow: TextOverflow.Ellipsis })
            .maxLines(1)
            .layoutWeight(1)
          //.margin({left: 10})
          if (this.isNext == true) {
            Image($r('app.media.icon_next'))
              .width(18)
              .height(18)
              .margin(10)
          }
        }.height(59)

        Divider().color($r('app.color.grey2')).strokeWidth(0.8)
      }
      .padding({ left: 10, right: 4 })
      .height(60)
      .layoutWeight(1)
    }
    .width('100%')
    .height(60)
    .margin({ top: this.marginTop })
    .backgroundColor(this.bgColor)
    .onTouch((event: TouchEvent) => {
      if (event.type === TouchType.Down) {
        this.bgColor = $r('app.color.grey2')
      }
      if (event.type === TouchType.Up) {
        this.bgColor = $r('app.color.white')
      }
    })
    .onClick((event: ClickEvent) => {
      this.itemOnClick(event)
    })
  }
}

@Component
export struct ClickText {
  @State bgColor: Resource = $r("app.color.blue_link")
  private img: Resource = $r("app.media.sun") //图标资源
  private context: string //文字内容
  private width: string = "30%" //组件上边距
  //  private marginTop: number   //组件上边距

  private itemOnClick = (event: ClickEvent) => {
  } //item的单击事件（可选）

  build() {
    Column() {
      Row() {
        Image(this.img)
          .width(30).height(30)
        Blank()
        Text(this.context)
          .fontFamily($r('sys.float.ohos_id_text_size_body1'))
          .fontSize(19)
          .textOverflow({ overflow: TextOverflow.Ellipsis })
          .maxLines(1)


      }.justifyContent(FlexAlign.Center)
      .layoutWeight(1)
    }
    .width(this.width)
    .height(60)
    .border(
      { width: 5 }
    )

    .backgroundColor(this.bgColor)
    .onTouch((event: TouchEvent) => {
      if (event.type === TouchType.Down) {
        this.bgColor = $r('app.color.emui4')
      }
      if (event.type === TouchType.Up) {
        this.bgColor = $r('app.color.blue_link')
      }
    })
    .onClick((event: ClickEvent) => {
      this.itemOnClick(event)
    })
  }
}

@Component
export struct TextRow {
  @State text: string = ''
  @State value: string = ''

  build() {
    Row() {
      Column() {
        Text(this.text)
          .fontSize(16)
          .fontColor(Color.White)
      }.width('32%')

      Column() {
        Text(this.value)
          .fontSize(16)
          .fontColor(Color.White)
      }.width('32%')
    }
  }
}