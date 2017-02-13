<GameProjectFile>
  <PropertyGroup Type="Node" Name="Instruction" ID="6b2e8f77-864c-42fb-b94f-bf0b874b7f91" Version="2.3.2.3" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="5" Speed="1.0000">
        <Timeline ActionTag="1310139174" Property="Position">
          <PointFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="5" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="1310139174" Property="Scale">
          <ScaleFrame FrameIndex="0" X="1.0000" Y="0.0001">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="5" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="1310139174" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="5" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <ObjectData Name="Node" Tag="41" ctype="GameNodeObjectData">
        <Size X="0.0000" Y="0.0000" />
        <Children>
          <AbstractNodeData Name="Black_Bg" ActionTag="1370389348" VisibleForFrame="False" Alpha="168" Tag="23" IconVisible="False" LeftMargin="-4.9088" RightMargin="-5.0912" TopMargin="-4.4316" BottomMargin="-5.5684" ctype="SpriteObjectData">
            <Size X="10.0000" Y="10.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="0.0912" Y="-0.5684" />
            <Scale ScaleX="108.4341" ScaleY="126.5684" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="Normal" Path="Com/Black_Bg.png" Plist="" />
            <BlendFunc Src="770" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="Instruction_Bg" ActionTag="1310139174" Tag="42" IconVisible="False" LeftMargin="-250.0000" RightMargin="-250.0000" TopMargin="-388.5000" BottomMargin="-388.5000" ctype="SpriteObjectData">
            <Size X="500.0000" Y="777.0000" />
            <Children>
              <AbstractNodeData Name="Instruction_Txt" ActionTag="2012333585" Tag="43" IconVisible="False" LeftMargin="51.5181" RightMargin="51.4819" TopMargin="48.2277" BottomMargin="115.7723" ctype="SpriteObjectData">
                <Size X="397.0000" Y="613.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="250.0181" Y="422.2723" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5435" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="PlistSubImage" Path="Instruction_Txt.png" Plist="Instruction.plist" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="Know_Btn" ActionTag="-2089729231" VisibleForFrame="False" Tag="44" IconVisible="False" LeftMargin="110.3550" RightMargin="109.6450" TopMargin="677.3265" BottomMargin="35.6735" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="250" Scale9Height="42" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="280.0000" Y="64.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="250.3550" Y="67.6735" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5007" Y="0.0871" />
                <PreSize X="0.0000" Y="0.0000" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="PlistSubImage" Path="Know_Btn_Down.png" Plist="Instruction.plist" />
                <PressedFileData Type="PlistSubImage" Path="Know_Btn_Down.png" Plist="Instruction.plist" />
                <NormalFileData Type="PlistSubImage" Path="Know_Btn_Up.png" Plist="Instruction.plist" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Text_Exit" ActionTag="387078453" Tag="6" IconVisible="False" LeftMargin="295.1890" RightMargin="44.8110" TopMargin="722.5469" BottomMargin="34.4532" FontSize="20" LabelText="点击任意位置关闭" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="160.0000" Y="20.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="375.1890" Y="44.4532" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7504" Y="0.0572" />
                <PreSize X="0.0000" Y="0.0000" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="Normal" Path="BigImg/BigScreen_Bg.png" Plist="" />
            <BlendFunc Src="770" Dst="771" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameProjectFile>