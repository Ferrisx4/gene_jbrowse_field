/*$("url_template_field").change(function(){
    alert("The text has been changed.");
    $("jbrowse_preview_link").innerHTML = "hello";
  }); */

jQuery(document).ready(function ($){


    // Update the link and preview on text field change
    $('#url_template_field').on('input',function(e){
        // Get the new value
        link_value = $('#url_template_field').val()

        // Run the string through the formatter
        link_value = template_formatter(link_value);

        // Finally, do the update
        update_link(link_value,$);
    });
});

// Create a "dictionary" of template options and their substitutions
var subs = new Object();
    subs["genus"]           = "fraxinus";
    subs["species"]         = "excelsior";
    subs["common name"]     = "European ash";
    subs["coords"]          = "71 42";
    subs["child sequence name"] = "some child";
    subs["gene name"]       = "wrangler";
    subs["analysis name"]   = "blue analysis";
    subs["g"]               = "f";
    subs["s"]               = "e";
    subs["G"]               = "F";
    subs["S"]               = "E";
    subs["Genus"]           = "Fraxinus";
    subs["Species"]         = "Species";

// Substitute any values from the available template options
function template_formatter(link_value)
{
    // Loop through the possible subs, regex replace when needed
    for(var key in subs)
    {
        //var myregex   = `/\\\\[${key}\\\\]/gi`;

        var replace = "\\\["+key+"\\\]";
        var re = new RegExp(replace,"g");
        link_value = link_value.replace(re,subs[key]);
    }

    return link_value;
}

// Update the preview link
function update_link(link_value,$)
{
    // Change the link text
    $("#jbrowse_preview_link").text(link_value);
    // Change the link itself
    $("#jbrowse_preview_link").attr("href", link_value);
}