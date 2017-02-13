<GameProjectFile>
  <PropertyGroup Type="Scene" Name="Loading" ID="19866cb7-508e-43f2-b923-bc4384c1d5b7" Version="2.3.2.3" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="100" Speed="1.0000">
        <Timeline ActionTag="835159411" Property="Position">
          <PointFrame FrameIndex="0" X="270.0001" Y="525.1779">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="100" X="270.0001" Y="525.1779">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="835159411" Property="Scale">
          <ScaleFrame FrameIndex="0" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="100" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="835159411" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="100" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-691918607" Property="Position">
          <PointFrame FrameIndex="0" X="271.0019" Y="471.3463">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="100" X="271.0011" Y="471.3441">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-691918607" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="-68.1343" Y="-68.1343">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="100" X="68.2866" Y="68.2866">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-691918607" Property="AnchorPoint">
          <ScaleFrame FrameIndex="0" X="0.5557" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <ObjectData Name="Scene" Tag="247" ctype="GameNodeObjectData">
        <Size X="540.0000" Y="960.0000" />
        <Children>
          <AbstractNodeData Name="MainUI_Bg" ActionTag="583284755" Tag="277" IconVisible="False" ctype="SpriteObjectData">
            <Size X="540.0000" Y="960.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="270.0000" Y="480.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="Normal" Path="BigImg/MainUI_Bg.jpg" Plist="" />
            <BlendFunc Src="770" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="Loading_Bg_1" ActionTag="-659504883" Tag="8" IconVisible="False" LeftMargin="32.4484" RightMargin="20.5516" TopMargin="64.1294" BottomMargin="378.8706" ctype="SpriteObjectData">
            <Size X="487.0000" Y="517.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="275.9484" Y="637.3706" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5110" Y="0.6639" />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="Normal" Path="BigImg/Loading_Bg.png" Plist="" />
            <BlendFunc Src="770" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="Loading_Bg" ActionTag="182722650" Tag="278" IconVisible="False" LeftMargin="0.0087" RightMargin="-0.0087" TopMargin="339.9985" BottomMargin="224.0015" ctype="SpriteObjectData">
            <Size X="540.0000" Y="396.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="270.0087" Y="422.0015" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.4396" />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="PlistSubImage" Path="Loading_Bg.png" Plist="Loading.plist" />
            <BlendFunc Src="770" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="Loading_Prog" ActionTag="835159411" Tag="281" IconVisible="False" LeftMargin="198.0001" RightMargin="197.9999" TopMargin="400.8221" BottomMargin="491.1779" ProgressInfo="100" ctype="LoadingBarObjectData">
            <Size X="144.0000" Y="68.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="270.0001" Y="525.1779" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5471" />
            <PreSize X="0.2667" Y="0.0708" />
            <ImageFileData Type="PlistSubImage" Path="Loading_Prog.png" Plist="Loading.plist" />
          </AbstractNodeData>
          <AbstractNodeData Name="Loading_Needle" ActionTag="-691918607" Tag="279" RotationSkewX="-68.1343" RotationSkewY="-68.1343" IconVisible="False" LeftMargin="260.9993" RightMargin="261.0007" TopMargin="404.6537" BottomMargin="471.3463" ctype="SpriteObjectData">
            <Size X="18.0000" Y="84.0000" />
            <AnchorPoint ScaleX="0.5557" />
            <Position X="271.0019" Y="471.3463" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5019" Y="0.4910" />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="PlistSubImage" Path="Loading_Needle.png" Plist="Loading.plist" />
            <BlendFunc Src="770" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="Loading_Cover" ActionTag="649913771" Tag="280" IconVisible="False" LeftMargin="183.5000" RightMargin="181.5000" TopMargin="473.6515" BottomMargin="394.3485" ctype="SpriteObjectData">
            <Size X="175.0000" Y="92.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="271.0000" Y="440.3485" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5019" Y="0.4587" />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="PlistSubImage" Path="Loading_Cover.png" Plist="Loading.plist" />
            <BlendFunc Src="770" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="Error_Info_Text" ActionTag="960825572" Tag="40" IconVisible="False" LeftMargin="225.0000" RightMargin="225.0000" TopMargin="896.0000" BottomMargin="46.0000" FontSize="18" LabelText="Error Info" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="90.0000" Y="18.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="270.0000" Y="55.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.0573" />
            <PreSize X="0.0000" Y="0.0000" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameProjectFile>