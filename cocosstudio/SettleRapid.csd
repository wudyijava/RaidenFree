<GameProjectFile>
  <PropertyGroup Type="Scene" Name="SettleRapid" ID="4432edd7-9fd6-4d0b-b278-c4d9fdbdcffb" Version="2.3.2.3" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="30" Speed="0.6667" ActivedAnimationName="ani1">
        <Timeline ActionTag="-1611259924" Property="Position">
          <PointFrame FrameIndex="0" X="270.0000" Y="1154.9077">
            <EasingData Type="14" />
          </PointFrame>
          <PointFrame FrameIndex="20" X="270.0000" Y="480.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="30" X="270.0000" Y="-199.0000">
            <EasingData Type="2" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-1611259924" Property="Scale">
          <ScaleFrame FrameIndex="0" X="1.0000" Y="1.0000">
            <EasingData Type="14" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="20" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="1.0000" Y="1.0000">
            <EasingData Type="2" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-1611259924" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="14" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="20" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="0.0000" Y="0.0000">
            <EasingData Type="2" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="ani1" StartIndex="0" EndIndex="20">
          <RenderColor A="255" R="139" G="0" B="139" />
        </AnimationInfo>
        <AnimationInfo Name="ani2" StartIndex="20" EndIndex="30">
          <RenderColor A="255" R="255" G="0" B="255" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Layer" Tag="13" ctype="GameLayerObjectData">
        <Size X="540.0000" Y="960.0000" />
        <Children>
          <AbstractNodeData Name="Bg" ActionTag="-1611259924" Tag="14" IconVisible="False" TopMargin="-390.4077" BottomMargin="959.4077" ctype="SpriteObjectData">
            <Size X="540.0000" Y="391.0000" />
            <Children>
              <AbstractNodeData Name="Quit_Btn" ActionTag="1718096201" Tag="15" IconVisible="False" LeftMargin="125.7310" RightMargin="304.2690" TopMargin="250.2253" BottomMargin="82.7747" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="80" Scale9Height="36" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="110.0000" Y="58.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="180.7310" Y="111.7747" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3347" Y="0.2859" />
                <PreSize X="0.2037" Y="0.1483" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="PlistSubImage" Path="Quit_Btn_Down.png" Plist="Common.plist" />
                <PressedFileData Type="PlistSubImage" Path="Quit_Btn_Down.png" Plist="Common.plist" />
                <NormalFileData Type="PlistSubImage" Path="Quit_Btn_Up.png" Plist="Common.plist" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Retry_Btn" ActionTag="-775011936" Tag="16" IconVisible="False" LeftMargin="308.0568" RightMargin="121.9432" TopMargin="250.2253" BottomMargin="82.7747" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="80" Scale9Height="36" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="110.0000" Y="58.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="363.0568" Y="111.7747" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.6723" Y="0.2859" />
                <PreSize X="0.0000" Y="0.0000" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="PlistSubImage" Path="Retry_Btn_Down.png" Plist="Settle.plist" />
                <PressedFileData Type="PlistSubImage" Path="Retry_Btn_Down.png" Plist="Settle.plist" />
                <NormalFileData Type="PlistSubImage" Path="Retry_Btn_Up.png" Plist="Settle.plist" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Current_Distance" ActionTag="415061180" Tag="17" IconVisible="False" LeftMargin="99.5707" RightMargin="145.4293" TopMargin="58.6609" BottomMargin="298.3391" ctype="SpriteObjectData">
                <Size X="295.0000" Y="34.0000" />
                <Children>
                  <AbstractNodeData Name="Current_Distance_Num" ActionTag="-1088652856" Tag="18" IconVisible="False" LeftMargin="122.0777" RightMargin="43.9223" TopMargin="-5.5623" BottomMargin="1.5623" LabelText="13249" ctype="TextBMFontObjectData">
                    <Size X="107.0000" Y="38.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="186.5777" Y="20.5623" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.6325" Y="0.6048" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <LabelBMFontFile_CNB Type="Normal" Path="Txt/Rapid_Distance.fnt" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="247.0707" Y="315.3391" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4575" Y="0.8065" />
                <PreSize X="0.5463" Y="0.0870" />
                <FileData Type="PlistSubImage" Path="Current_Distance.png" Plist="Settle.plist" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="Best_Distance" ActionTag="1913938092" Tag="19" IconVisible="False" LeftMargin="99.5707" RightMargin="145.4293" TopMargin="118.8044" BottomMargin="238.1956" ctype="SpriteObjectData">
                <Size X="295.0000" Y="34.0000" />
                <Children>
                  <AbstractNodeData Name="Best_Distance_Num" ActionTag="1800157611" Tag="20" IconVisible="False" LeftMargin="122.0777" RightMargin="43.9223" TopMargin="-5.4471" BottomMargin="1.4471" LabelText="13249" ctype="TextBMFontObjectData">
                    <Size X="107.0000" Y="38.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="186.5777" Y="20.4471" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.6325" Y="0.6014" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <LabelBMFontFile_CNB Type="Normal" Path="Txt/Rapid_Distance.fnt" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="247.0707" Y="255.1956" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4575" Y="0.6527" />
                <PreSize X="0.5463" Y="0.0870" />
                <FileData Type="PlistSubImage" Path="Best_Distance.png" Plist="Settle.plist" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="Distance_Rank" ActionTag="-1715594989" Tag="21" IconVisible="False" LeftMargin="99.5708" RightMargin="93.4292" TopMargin="180.9264" BottomMargin="176.0736" ctype="SpriteObjectData">
                <Size X="347.0000" Y="34.0000" />
                <Children>
                  <AbstractNodeData Name="Distance_Rank_Num" ActionTag="1701150567" Tag="22" IconVisible="False" LeftMargin="122.6570" RightMargin="90.3430" TopMargin="-5.4669" BottomMargin="1.4669" LabelText="99.9%" ctype="TextBMFontObjectData">
                    <Size X="112.0000" Y="38.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="189.6570" Y="20.4669" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5466" Y="0.6020" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <LabelBMFontFile_CNB Type="Normal" Path="Txt/Rapid_Distance.fnt" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="273.0708" Y="193.0736" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5057" Y="0.4938" />
                <PreSize X="0.6426" Y="0.0870" />
                <FileData Type="PlistSubImage" Path="Distance_Rank.png" Plist="Settle.plist" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="Connecting_Web" Visible="False" ActionTag="781126089" Tag="23" IconVisible="False" LeftMargin="101.4377" RightMargin="129.5623" TopMargin="182.1595" BottomMargin="178.8405" ctype="SpriteObjectData">
                <Size X="309.0000" Y="30.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="255.9377" Y="193.8405" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4740" Y="0.4958" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="PlistSubImage" Path="Connecting_Web.png" Plist="Settle.plist" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="Get_Rank_Failed" Visible="False" ActionTag="-1879600492" Tag="24" IconVisible="False" LeftMargin="100.1113" RightMargin="100.8887" TopMargin="183.7216" BottomMargin="175.2784" ctype="SpriteObjectData">
                <Size X="339.0000" Y="32.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="269.6113" Y="191.2784" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4993" Y="0.4892" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="PlistSubImage" Path="Get_Rank_Failed.png" Plist="Settle.plist" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="270.0000" Y="1154.9077" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="1.2030" />
            <PreSize X="1.0000" Y="0.4073" />
            <FileData Type="PlistSubImage" Path="Buyplane_Bg.png" Plist="Common.plist" />
            <BlendFunc Src="770" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="Get_Piece_Txt" ActionTag="2022597670" Tag="41" IconVisible="False" LeftMargin="209.5000" RightMargin="209.5000" TopMargin="679.5000" BottomMargin="259.5000" ctype="SpriteObjectData">
            <Size X="121.0000" Y="21.0000" />
            <Children>
              <AbstractNodeData Name="Node_Piece" ActionTag="-560554055" Tag="42" IconVisible="True" LeftMargin="60.0000" RightMargin="61.0000" TopMargin="81.0000" BottomMargin="-60.0000" ctype="SingleNodeObjectData">
                <Size X="0.0000" Y="0.0000" />
                <AnchorPoint />
                <Position X="60.0000" Y="-60.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4959" Y="-2.8571" />
                <PreSize X="0.0000" Y="0.0000" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="270.0000" Y="270.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.2813" />
            <PreSize X="0.2241" Y="0.0219" />
            <FileData Type="PlistSubImage" Path="Get_Piece_Txt.png" Plist="Settle.plist" />
            <BlendFunc Src="770" Dst="771" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameProjectFile>