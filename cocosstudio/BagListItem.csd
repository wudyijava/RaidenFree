<GameProjectFile>
  <PropertyGroup Type="Node" Name="BagListItem" ID="b0a34d4b-8be4-4102-8fef-13ae3207a27a" Version="2.3.2.3" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="60" Speed="1.0000">
        <Timeline ActionTag="-844281693" Property="Position">
          <PointFrame FrameIndex="30" X="14.0800" Y="14.4300">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="55" X="14.0800" Y="14.4300">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-844281693" Property="Scale">
          <ScaleFrame FrameIndex="30" X="0.0001" Y="0.0001">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="55" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="1.3000" Y="1.3000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-844281693" Property="RotationSkew">
          <ScaleFrame FrameIndex="30" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-844281693" Property="Alpha">
          <IntFrame FrameIndex="30" Value="0">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="55" Value="255">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="60" Value="0">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
      </Animation>
      <ObjectData Name="Node" Tag="421" ctype="GameNodeObjectData">
        <Size X="0.0000" Y="0.0000" />
        <Children>
          <AbstractNodeData Name="Bag_Bg_Little1" ActionTag="479252122" Tag="422" IconVisible="False" LeftMargin="-230.5000" RightMargin="-230.5000" TopMargin="-64.0000" BottomMargin="-64.0000" ctype="SpriteObjectData">
            <Size X="461.0000" Y="128.0000" />
            <Children>
              <AbstractNodeData Name="Bag_Name_1" ActionTag="-78774757" Tag="104" IconVisible="False" LeftMargin="108.3643" RightMargin="272.6357" TopMargin="24.3781" BottomMargin="83.6219" FontSize="20" LabelText="装备名称" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="80.0000" Y="20.0000" />
                <AnchorPoint ScaleY="0.5000" />
                <Position X="108.3643" Y="93.6219" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2351" Y="0.7314" />
                <PreSize X="0.0000" Y="0.0000" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Bag_Prop_Img" ActionTag="1097779519" Tag="424" IconVisible="False" LeftMargin="2.1165" RightMargin="338.8835" TopMargin="3.7059" BottomMargin="4.2941" ctype="SpriteObjectData">
                <Size X="120.0000" Y="120.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="62.1165" Y="64.2941" />
                <Scale ScaleX="0.7000" ScaleY="0.7000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1347" Y="0.5023" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="PlistSubImage" Path="Buyplane_Box_Img.png" Plist="Common.plist" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="Bag_Strengthen_Txt" ActionTag="-934055648" Tag="105" IconVisible="False" LeftMargin="108.3716" RightMargin="320.6284" TopMargin="58.7108" BottomMargin="53.2892" FontSize="16" LabelText="属性" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="32.0000" Y="16.0000" />
                <Children>
                  <AbstractNodeData Name="Bag_Strengthen_Num" ActionTag="-1632007878" Tag="426" IconVisible="False" LeftMargin="40.5906" RightMargin="-58.5906" TopMargin="0.4491" BottomMargin="0.5509" CharWidth="10" CharHeight="15" LabelText="99999" StartChar="." ctype="TextAtlasObjectData">
                    <Size X="50.0000" Y="15.0000" />
                    <AnchorPoint ScaleY="0.5035" />
                    <Position X="40.5906" Y="8.1034" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="1.2685" Y="0.5065" />
                    <PreSize X="0.0019" Y="0.0188" />
                    <LabelAtlasFileImage_CNB Type="Normal" Path="Com/MainUI_Num2.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Bag_Prog" ActionTag="-1350810213" Tag="427" IconVisible="False" LeftMargin="100.1192" RightMargin="-186.1192" TopMargin="-0.8242" BottomMargin="-0.1758" ProgressInfo="100" ctype="LoadingBarObjectData">
                    <Size X="118.0000" Y="17.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="159.1192" Y="8.3242" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="4.9725" Y="0.5203" />
                    <PreSize X="0.2185" Y="0.0177" />
                    <ImageFileData Type="PlistSubImage" Path="Bag_Prog.png" Plist="Bag.plist" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="0.5000" />
                <Position X="108.3716" Y="61.2892" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="30" G="144" B="255" />
                <PrePosition X="0.2351" Y="0.4788" />
                <PreSize X="0.0694" Y="0.1250" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Bag_Strengthen_Btn" ActionTag="-1084613215" Tag="428" IconVisible="False" LeftMargin="332.9840" RightMargin="17.0160" TopMargin="38.1365" BottomMargin="31.8635" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="81" Scale9Height="36" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="111.0000" Y="58.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="388.4840" Y="60.8635" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8427" Y="0.4755" />
                <PreSize X="0.2408" Y="0.4531" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="PlistSubImage" Path="Bag_Upgrade_Btn_Down.png" Plist="Bag.plist" />
                <PressedFileData Type="PlistSubImage" Path="Bag_Upgrade_Btn_Down.png" Plist="Bag.plist" />
                <NormalFileData Type="PlistSubImage" Path="Bag_Upgrade_Btn_Up.png" Plist="Bag.plist" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Bag_Equip_Btn" ActionTag="-1828442485" Tag="432" IconVisible="False" LeftMargin="332.9841" RightMargin="17.0159" TopMargin="41.1364" BottomMargin="30.8636" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="81" Scale9Height="34" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="111.0000" Y="56.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="388.4841" Y="58.8636" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8427" Y="0.4599" />
                <PreSize X="0.2408" Y="0.4375" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="PlistSubImage" Path="Bag_Equip_Btn_Down.png" Plist="Bag.plist" />
                <PressedFileData Type="PlistSubImage" Path="Bag_Equip_Btn_Down.png" Plist="Bag.plist" />
                <NormalFileData Type="PlistSubImage" Path="Bag_Equip_Btn_Up.png" Plist="Bag.plist" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Bag_Buy_Btn" ActionTag="1873923347" Tag="434" IconVisible="False" LeftMargin="332.9841" RightMargin="17.0159" TopMargin="41.1364" BottomMargin="30.8636" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="81" Scale9Height="34" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="111.0000" Y="56.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="388.4841" Y="58.8636" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8427" Y="0.4599" />
                <PreSize X="0.2408" Y="0.4375" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="PlistSubImage" Path="Bag_Buy_Btn_Down.png" Plist="Bag.plist" />
                <PressedFileData Type="PlistSubImage" Path="Bag_Buy_Btn_Down.png" Plist="Bag.plist" />
                <NormalFileData Type="PlistSubImage" Path="Bag_Buy_Btn_Up.png" Plist="Bag.plist" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Bag_Compose_Btn" ActionTag="1963302121" Tag="16" IconVisible="False" LeftMargin="333.5000" RightMargin="16.5000" TopMargin="41.5000" BottomMargin="33.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="81" Scale9Height="31" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="111.0000" Y="53.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="389.0000" Y="60.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8438" Y="0.4688" />
                <PreSize X="0.2408" Y="0.4141" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="PlistSubImage" Path="Bag_Compose_Btn_Down.png" Plist="Bag.plist" />
                <PressedFileData Type="PlistSubImage" Path="Bag_Compose_Btn_Down.png" Plist="Bag.plist" />
                <NormalFileData Type="PlistSubImage" Path="Bag_Compose_Btn_Up.png" Plist="Bag.plist" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Bag_Message_Bg" ActionTag="-919200589" Tag="63" IconVisible="False" LeftMargin="421.1051" RightMargin="11.8949" TopMargin="27.9949" BottomMargin="72.0051" ctype="SpriteObjectData">
                <Size X="28.0000" Y="28.0000" />
                <Children>
                  <AbstractNodeData Name="Bag_Message_Light" ActionTag="-844281693" Alpha="101" Tag="64" IconVisible="False" LeftMargin="-2.9200" RightMargin="-3.0800" TopMargin="-3.4300" BottomMargin="-2.5700" ctype="SpriteObjectData">
                    <Size X="34.0000" Y="34.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="14.0800" Y="14.4300" />
                    <Scale ScaleX="1.1800" ScaleY="1.1800" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5029" Y="0.5154" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="MainUI_Reward_Message_Light.png" Plist="MainUI.plist" />
                    <BlendFunc Src="770" Dst="771" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="435.1051" Y="86.0051" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9438" Y="0.6719" />
                <PreSize X="0.0607" Y="0.2188" />
                <FileData Type="PlistSubImage" Path="MainUI_Reward_Message_Bg.png" Plist="MainUI.plist" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="Bag_Equip_Txt" ActionTag="-1250316605" Tag="429" IconVisible="False" LeftMargin="246.5457" RightMargin="160.4543" TopMargin="27.1984" BottomMargin="84.8016" ctype="SpriteObjectData">
                <Size X="54.0000" Y="16.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="273.5457" Y="92.8016" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5934" Y="0.7250" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="PlistSubImage" Path="Bag_Equip_Txt1.png" Plist="Bag.plist" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="Bag_Piece_Txt" ActionTag="85882183" Tag="184" IconVisible="False" LeftMargin="224.0527" RightMargin="181.9473" TopMargin="85.5963" BottomMargin="26.4037" ctype="SpriteObjectData">
                <Size X="55.0000" Y="16.0000" />
                <Children>
                  <AbstractNodeData Name="Bag_Pick_Cnt_Txt" ActionTag="1629108757" Tag="185" IconVisible="False" LeftMargin="56.7786" RightMargin="-51.7786" TopMargin="0.9647" BottomMargin="0.0353" CharWidth="10" CharHeight="15" LabelText="30/30" StartChar="." ctype="TextAtlasObjectData">
                    <Size X="50.0000" Y="15.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="56.7786" Y="7.5353" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="1.0323" Y="0.4710" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <LabelAtlasFileImage_CNB Type="Normal" Path="Com/MainUI_Num2.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="251.5527" Y="34.4037" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5457" Y="0.2688" />
                <PreSize X="0.1193" Y="0.1250" />
                <FileData Type="PlistSubImage" Path="Bag_Strengthen_Txt3.png" Plist="Bag.plist" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="Bag_Diamond_Img" ActionTag="-378231525" Tag="430" IconVisible="False" LeftMargin="103.5206" RightMargin="312.4794" TopMargin="67.3389" BottomMargin="12.6611" ctype="SpriteObjectData">
                <Size X="45.0000" Y="48.0000" />
                <Children>
                  <AbstractNodeData Name="Bag_Num" ActionTag="-1540479877" CallBackType="Touch" Tag="431" IconVisible="False" LeftMargin="60.6525" RightMargin="-99.6525" TopMargin="17.7471" BottomMargin="12.2529" CharWidth="12" CharHeight="18" LabelText="99999" StartChar="." ctype="TextAtlasObjectData">
                    <Size X="60.0000" Y="18.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="60.6525" Y="21.2529" />
                    <Scale ScaleX="1.3500" ScaleY="1.3500" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="1.3478" Y="0.4428" />
                    <PreSize X="0.2870" Y="0.0188" />
                    <LabelAtlasFileImage_CNB Type="Normal" Path="Com/Shop_Num.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="126.0206" Y="36.6611" />
                <Scale ScaleX="0.6000" ScaleY="0.6000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2734" Y="0.2864" />
                <PreSize X="0.0976" Y="0.3750" />
                <FileData Type="Normal" Path="Com/MainUI_Diamond_Img.png" Plist="" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="PlistSubImage" Path="Bag_Bg_Little1.png" Plist="Bag.plist" />
            <BlendFunc Src="770" Dst="771" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameProjectFile>