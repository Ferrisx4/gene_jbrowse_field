// Create a "dictionary" of available tokens and their example substitutions
var subs = new Object();
    subs["genus"]           = "fraxinus";
    subs["species"]         = "excelsior";
    subs["common name"]     = "European ash";
    subs["coords"]          = "226460..236641";
    subs["child sequence name"] = "OFAS011230-RA";
    subs["gene name"]       = "OFAS019435";
    subs["analysis name"]   = "blue analysis";
    subs["g"]               = "f";
    subs["s"]               = "e";
    subs["G"]               = "F";
    subs["S"]               = "E";
    subs["Genus"]           = "Fraxinus";
    subs["Species"]         = "Excelsior";
/*$("url_template_field").change(function(){
    alert("The text has been changed.");
    $("jbrowse_preview_link").innerHTML = "hello";
  }); */

var url = window.location.href;
if (url.includes("gene_jbrowse_field/config"))
{
    jQuery(document).ready(function ($){
        // Are we on the gene_jbrwose_field config page?
        // TODO: make this more flexible to support n number of fields
        main($);
        // Update the link and preview on text field change
        $('#url_template_field').on('input',function(e){
            main($);
        });
        // Same for the mRNA field
        $('#mrna_url_template_field').on('input',function(e){
            main($);
        });
    });
}

// Main (the meat and potatoes)
function main($)
{
    // Gene
    // Get the new value
    gene_link_value = $('#url_template_field').val()
    // Run the string through the formatter
    gene_link_value = template_formatter(gene_link_value);
    // Finally, do the update
    update_link('gene', gene_link_value,$);

    // Same for mRNA
    // Get the new value
    mrna_link_value = $('#mrna_url_template_field').val()
    // Run the string through the formatter
    mrna_link_value = template_formatter(mrna_link_value);
    // Finally, do the update
    update_link('mrna',mrna_link_value,$);
}


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
function update_link(field, link_value,$)
{
    // Change the link text
    id = "#jbrowse_preview_link_" + field;
    $(id).text(link_value);
    // Change the link itself
    $(id).attr("href", link_value);
}