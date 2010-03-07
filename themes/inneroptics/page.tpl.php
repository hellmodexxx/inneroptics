<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
  <title><?php print $head_title ?></title>
  <?php print $head ?>
  <?php print $styles ?>
  	
  <meta name="verify-v1" content="eK7TY6/1yt2cIqUJK98H8zdA/wZa4UhMfzeda+uXWAg=" />
  <link rel="stylesheet" type="text/css" href="<?php print $base_path ?>css/base.css" />
  <script type="text/javascript"><?php /* Needed to avoid Flash of Unstyle Content in IE */ ?> </script>
  
  <?php if ($headerBlocks) { ?>
  <script type="text/javascript" src="<?php print $base_path ?>js/prototype.lite.js"></script>
  <script type="text/javascript" src="<?php print $base_path ?>js/moo.fx.js"></script>
  

  <?php } ?>
</head>

<body<?php print $body_class ? ' class="'. $body_class .'"' : ''; ?>>
    <?php if ($headerBlocks) { ?>

    <div id="headerBlocks">
      <?php print $headerBlocks; ?>        
    </div>
    <div id="headerBlocksButton"><a name="top"></a><a href="javascript:void(0);" onclick="myHeight.toggle();">Show/Hide</a></div>
    <?php } ?>

    <div id="body-bg">
      <div id="wrapper">
               
        <div id="header">
          <h1><a href="<?php print $base_path ?>"><span><?php print $site_name ?></span></a></h1>
          <h2><?php print $site_slogan ?></h2> 
        </div>

        <div id="menu">
          <ul>
          <?php foreach ($primary_links as $link) {?>
            <?php print $link; ?>
          <?php } ?>
          </ul>
        </div>

        <div id="content">
          <div id="columns"<?php print !$sidebar_right ? ' class="no-right"' : ''; ?>>
            <div id="left-col" class="column content">
              <!-- <div id="breadcrumb">
                <?php print $breadcrumb ?>
              </div> -->
              

            
                <!-- <h2 class="title"><?php print $title ?></h2> -->
                <div class="tabs"><?php print $tabs ?></div>
                <?php print $messages ?>
                <?php print $content; ?>
              

            </div>
            
<?php if ($sidebar_right) { ?>
            <div id="right-col" class="column content">
              <?php print $sidebar_right ?>
            </div>
<?php } ?>
          </div><!-- //#columns -->
        </div><!-- //#content -->
        
<?php if ($bottomBlocks) { ?>
        <div id="threeColBottom">
          <?php print $bottomBlocks; ?>
          <div class="clear"></div>    
        </div>
<?php } ?>
        
        <div id="footer">
          <?php print $footer_message; ?>
        </div>
      </div><!-- #wrapper div -->
    </div><!-- #body-bg div -->
    <?php print $closure; ?>
  </body>
</html>
