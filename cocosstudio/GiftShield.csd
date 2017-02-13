<GameProjectFile>
  <PropertyGroup Type="Node" Name="GiftShield" ID="18c8c054-6d0a-4d4c-8d33-029f7dbeca22" Version="2.3.2.3" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Node" Tag="41" ctype="GameNodeObjectData">
        <Size X="0.0000" Y="0.0000" />
        <Children>
          <AbstractNodeData Name="Gift_Bg" ActionTag="-827024400" Tag="43" IconVisible="False" LeftMargin="-220.0000" RightMargin="-220.0000" TopMargin="-210.0000" BottomMargin="-210.0000" ctype="SpriteObjectData">
            <Size X="440.0000" Y="420.0000" />
            <Children>
              <AbstractNodeData Name="Close_Btn" ActionTag="1672184368" Tag="44" IconVisible="False" LeftMargin="347.1844" RightMargin="12.8156" TopMargin="105.7055" BottomMargin="269.2945" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="50" Scale9Height="23" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="80.0000" Y="45.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="387.1844" Y="291.7945" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8800" Y="0.6947" />
                <PreSize X="0.1818" Y="0.1071" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="PlistSubImage" Path="Close_Btn_Down.png" Plist="Common.plist" />
                <PressedFileData Type="PlistSubImage" Path="Close_Btn_Down.png" Plist="Common.plist" />
                <NormalFileData Type="PlistSubImage" Path="Close_Btn_Up.png" Plist="Common.plist" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Gift_Buy_Btn" ActionTag="589306613" Tag="46" IconVisible="False" LeftMargin="132.2393" RightMargin="130.7607" TopMargin="344.6931" BottomMargin="12.3069" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="147" Scale9Height="41" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="177.0000" Y="63.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="220.7393" Y="43.8069" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5017" Y="0.1043" />
                <PreSize X="0.0000" Y="0.0000" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="PlistSubImage" Path="Gift_Buy_Btn_Down.png" Plist="Gift.plist" />
                <PressedFileData Type="PlistSubImage" Path="Gift_Buy_Btn_Down.png" Plist="Gift.plist" />
                <NormalFileData Type="PlistSubImage" Path="Gift_Buy_Btn_Up.png" Plist="Gift.plist" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Gift_Title" ActionTag="1041486843" Tag="25" IconVisible="False" LeftMargin="179.6627" RightMargin="178.3373" TopMargin="74.7436" BottomMargin="309.2564" ctype="SpriteObjectData">
                <Size X="83.0000" Y="38.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="220.6627" Y="327.2564" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5015" Y="0.7792" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="PlistSubImage" Path="Gift_Title_Gift.png" Plist="Gift.plist" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="BitmapFontLabel" ActionTag="1158101269" Tag="27" IconVisible="False" LeftMargin="240.3779" RightMargin="126.6221" TopMargin="195.4663" BottomMargin="193.5337" LabelText="护盾x" ctype="TextBMFontObjectData">
                <Size X="73.0000" Y="31.0000" />
                <Children>
                  <AbstractNodeData Name="Shield_Num_Txt" ActionTag="-418779768" Tag="28" IconVisible="False" LeftMargin="73.3590" RightMargin="-73.3590" TopMargin="0.9977" BottomMargin="-0.9977" LabelText="5" ctype="TextBMFontObjectData">
                    <Size X="16.0000" Y="31.0000" />
                    <AnchorPoint ScaleY="0.5660" />
                    <Position X="73.3590" Y="16.5483" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="1.0049" Y="0.5338" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <LabelBMFontFile_CNB Type="Normal" Path="Txt/GiftText.fnt" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="276.8779" Y="209.0337" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.6293" Y="0.4977" />
                <PreSize X="0.1659" Y="0.0738" />
                <LabelBMFontFile_CNB Type="Normal" Path="Txt/GiftText.fnt" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="Gift_Prop_Shield_Img" ActionTag="-2104195856" Tag="26" IconVisible="False" LeftMargin="79.3573" RightMargin="246.6427" TopMargin="174.4850" BottomMargin="131.5150" ctype="SpriteObjectData">
                <Size X="114.0000" Y="114.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="136.3573" Y="188.5150" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3099" Y="0.4488" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="PlistSubImage" Path="Gift_Prop_Shield_Img.png" Plist="Gift.plist" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="Gift_Rmb_Img" ActionTag="1229218483" Tag="48" IconVisible="False" LeftMargin="251.2247" RightMargin="174.7753" TopMargin="251.9378" BottomMargin="149.0622" ctype="SpriteObjectData">
                <Size X="14.0000" Y="19.0000" />
                <Children>
                  <AbstractNodeData Name="Gift_Num1" ActionTag="2073626949" Tag="49" IconVisible="False" LeftMargin="20.9151" RightMargin="-18.9151" TopMargin="0.6008" BottomMargin="0.3992" CharWidth="12" CharHeight="18" LabelText="2" StartChar="." ctype="TextAtlasObjectData">
                    <Size X="12.0000" Y="18.0000" />
                    <AnchorPoint ScaleX="1.0000" ScaleY="0.4650" />
                    <Position X="32.9151" Y="8.7692" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="2.3511" Y="0.4615" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <LabelAtlasFileImage_CNB Type="Normal" Path="Com/Shop_Num.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Gift_Num2" ActionTag="-2043099192" Tag="50" IconVisible="False" LeftMargin="31.9149" RightMargin="-53.9149" TopMargin="2.5999" BottomMargin="-1.5999" CharWidth="12" CharHeight="18" LabelText=".00" StartChar="." ctype="TextAtlasObjectData">
                    <Size X="36.0000" Y="18.0000" />
                    <AnchorPoint ScaleY="0.4974" />
                    <Position X="31.9149" Y="7.3533" />
                    <Scale ScaleX="0.8000" ScaleY="0.8000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="2.2796" Y="0.3870" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <LabelAtlasFileImage_CNB Type="Normal" Path="Com/Shop_Num.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="258.2247" Y="158.5622" />
                <Scale ScaleX="1.4000" ScaleY="1.4000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5869" Y="0.3775" />
                <PreSize X="0.0318" Y="0.0452" />
                <FileData Type="PlistSubImage" Path="Shop_Rmb_Img.png" Plist="Common.plist" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="PlistSubImage" Path="Gift_Bg.png" Plist="Gift.plist" />
            <BlendFunc Src="770" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="SpriteBuyFail" ActionTag="-34788884" Tag="552" IconVisible="False" LeftMargin="-66.0000" RightMargin="-66.0000" TopMargin="263.0000" BottomMargin="-297.0000" ctype="SpriteObjectData">
            <Size X="132.0000" Y="34.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position Y="-280.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="PlistSubImage" Path="buy-fail.png" Plist="Common.plist" />
            <BlendFunc Src="770" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="SpriteBuySuccess" ActionTag="-1564398849" Tag="553" IconVisible="False" LeftMargin="-65.5000" RightMargin="-65.5000" TopMargin="263.0000" BottomMargin="-297.0000" ctype="SpriteObjectData">
            <Size X="131.0000" Y="34.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position Y="-280.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="PlistSubImage" Path="buy-success.png" Plist="Common.plist" />
            <BlendFunc Src="770" Dst="771" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameProjectFile>