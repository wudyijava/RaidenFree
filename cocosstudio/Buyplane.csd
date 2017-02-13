<GameProjectFile>
  <PropertyGroup Type="Scene" Name="Buyplane" ID="a8640b8d-70d9-420d-8ed1-5775887ab68b" Version="2.3.2.3" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="30" Speed="1.0000" ActivedAnimationName="and_2">
        <Timeline ActionTag="558250046" Property="Position">
          <PointFrame FrameIndex="10" X="270.0000" Y="-150.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="15" X="270.0000" Y="0.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="20" X="270.0000" Y="-150.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="558250046" Property="Scale">
          <ScaleFrame FrameIndex="15" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="20" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="558250046" Property="RotationSkew">
          <ScaleFrame FrameIndex="15" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="20" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-29339938" Property="Position">
          <PointFrame FrameIndex="0" X="270.0000" Y="1150.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="10" X="270.0000" Y="480.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="15" X="270.0000" Y="480.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="20" X="270.0000" Y="480.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="30" X="270.0000" Y="-190.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-29339938" Property="Scale">
          <ScaleFrame FrameIndex="0" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="10" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="15" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="20" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-29339938" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="10" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="15" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="20" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="ani_1" StartIndex="0" EndIndex="15">
          <RenderColor A="255" R="95" G="158" B="160" />
        </AnimationInfo>
        <AnimationInfo Name="and_2" StartIndex="15" EndIndex="30">
          <RenderColor A="255" R="253" G="245" B="230" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Scene" Tag="71" ctype="GameNodeObjectData">
        <Size X="540.0000" Y="960.0000" />
        <Children>
          <AbstractNodeData Name="Black_Bg" ActionTag="1434375279" Alpha="168" Tag="77" IconVisible="False" LeftMargin="265.0000" RightMargin="265.0000" TopMargin="475.0000" BottomMargin="475.0000" ctype="SpriteObjectData">
            <Size X="10.0000" Y="10.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="270.0000" Y="480.0000" />
            <Scale ScaleX="62.0338" ScaleY="102.1679" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="Normal" Path="Com/Black_Bg.png" Plist="" />
            <BlendFunc Src="770" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="Reward_Back_Bg" ActionTag="558250046" Tag="104" IconVisible="False" TopMargin="823.0000" ctype="SpriteObjectData">
            <Size X="540.0000" Y="137.0000" />
            <Children>
              <AbstractNodeData Name="Back_Btn" ActionTag="750808112" Tag="105" IconVisible="False" LeftMargin="403.9177" RightMargin="17.0823" TopMargin="-14.7111" BottomMargin="82.7111" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="89" Scale9Height="47" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="119.0000" Y="69.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="463.4177" Y="117.2111" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8582" Y="0.8556" />
                <PreSize X="0.2204" Y="0.5036" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="PlistSubImage" Path="Back_Btn_Down.png" Plist="Common.plist" />
                <PressedFileData Type="PlistSubImage" Path="Back_Btn_Down.png" Plist="Common.plist" />
                <NormalFileData Type="PlistSubImage" Path="Back_Btn_Up.png" Plist="Common.plist" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" />
            <Position X="270.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" />
            <PreSize X="1.0000" Y="0.1427" />
            <FileData Type="PlistSubImage" Path="Reward_Back_Bg.png" Plist="Common.plist" />
            <BlendFunc Src="770" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="Buyplane_Bg1" ActionTag="-29339938" Tag="106" IconVisible="False" TopMargin="284.5000" BottomMargin="284.5000" ctype="SpriteObjectData">
            <Size X="540.0000" Y="391.0000" />
            <Children>
              <AbstractNodeData Name="Buyplane_Bg2" ActionTag="-1782875545" Tag="107" IconVisible="False" LeftMargin="209.9999" RightMargin="210.0001" TopMargin="47.9994" BottomMargin="223.0006" ctype="SpriteObjectData">
                <Size X="120.0000" Y="120.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="269.9999" Y="283.0006" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.7238" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="PlistSubImage" Path="Buyplane_Box_Img.png" Plist="Common.plist" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="Buy_Btn" ActionTag="-640617510" Tag="108" IconVisible="False" LeftMargin="209.4999" RightMargin="209.5001" TopMargin="274.0006" BottomMargin="72.9994" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="91" Scale9Height="22" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="121.0000" Y="44.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="269.9999" Y="94.9994" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.2430" />
                <PreSize X="0.2241" Y="0.1125" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="PlistSubImage" Path="Shop_Buy_Btn_Down.png" Plist="Common.plist" />
                <PressedFileData Type="PlistSubImage" Path="Shop_Buy_Btn_Down.png" Plist="Common.plist" />
                <NormalFileData Type="PlistSubImage" Path="Shop_Buy_Btn_Up.png" Plist="Common.plist" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Diamond_Img" ActionTag="-1473857310" Tag="109" IconVisible="False" LeftMargin="195.3113" RightMargin="299.6887" TopMargin="220.1501" BottomMargin="122.8499" ctype="SpriteObjectData">
                <Size X="45.0000" Y="48.0000" />
                <Children>
                  <AbstractNodeData Name="Diamond_Num" ActionTag="-1251193813" Tag="110" IconVisible="False" LeftMargin="43.6472" RightMargin="-166.6472" TopMargin="13.4896" BottomMargin="10.5104" CharWidth="14" CharHeight="24" LabelText="1234567" StartChar="." ctype="TextAtlasObjectData">
                    <Size X="98.0000" Y="24.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="43.6472" Y="22.5104" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9699" Y="0.4690" />
                    <PreSize X="0.3111" Y="0.0188" />
                    <LabelAtlasFileImage_CNB Type="Normal" Path="Com/MainUI_Num1.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="217.8113" Y="146.8499" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4034" Y="0.3756" />
                <PreSize X="0.0833" Y="0.1228" />
                <FileData Type="PlistSubImage" Path="MainUI_Diamond_Img.png" Plist="Common.plist" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="270.0000" Y="480.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="0.4073" />
            <FileData Type="PlistSubImage" Path="Buyplane_Bg.png" Plist="Common.plist" />
            <BlendFunc Src="770" Dst="771" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameProjectFile>