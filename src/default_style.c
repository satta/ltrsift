/*
  Copyright (c) 2012 Sascha Steinbiss <steinbiss@zbh.uni-hamburg.de>
  Copyright (c) 2012 Center for Bioinformatics, University of Hamburg

  Permission to use, copy, modify, and distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "default_style.h"

static char *default_style = "--[[\n"
  "  This is the GenomeTools annotation viewer config file.\n"
  "  All options must be set inside the 'style' table.\n"
  "  All items in this table must be tables, called 'sections'.\n"
  "  Typically there is one section per feature type.\n"
  "\n"
  "  Color definitions\n"
  "  -----------------\n"
  "  Colors are given as RGB triplets\n"
  "       {red=<val>,green=<val>,blue=<val>(,alpha=<val>)}\n"
  "  where <val> is a decimal value between 0 and 1.\n"
  "  The alpha value is optional, as a default 0.5 will be used.\n"
  "\n"
  "  Global format settings\n"
  "  ----------------------\n"
  "  Global settings, such as margins, spacers, etc. are found at the end of this\n"
  "  file, in the \"format\" section.\n"
  "]]--\n"
  "\n"
  "\n"
  "style =\n"
  "{\n"
  "  gene = {\n"
  "    -- Color definitions\n"
  "    stroke             = {red=0.0, green=0.0, blue=0.0, alpha = 0.6},\n"
  "    stroke_marked      = {red=1.0, green=0.0, blue=0.0, alpha = 0.6},\n"
  "    fill               = {red=0.9, green=0.9, blue=1.0, alpha = 0.6},\n"
  "    style              = \"box\",\n"
  "    -- Collapsing options\n"
  "    collapse_to_parent = false,\n"
  "    split_lines        = true,\n"
  "    -- Caption options\n"
  "    max_capt_show_width= nil,\n"
  "    -- Display this track only if the viewport is not wider than this\n"
  "    -- number of nucleotides. Set to 0 to disable type track.\n"
  "    max_show_width     = nil,\n"
  "    -- Limit the number of tracks\n"
  "    -- max_num_lines      = 10,\n"
  "  },\n"
  "--------------------------------------\n"
  "  mRNA = {\n"
  "    -- Color definitions\n"
  "    stroke             = {red=0.0, green=0.0, blue=0.0, alpha = 0.1},\n"
  "    stroke_marked      = {red=1.0, green=0.0, blue=0.0},\n"
  "    fill               = {red=0.4, green=0.5, blue=0.6, alpha=0.3},\n"
  "    style              = \"box\",\n"
  "    -- Collapsing options\n"
  "    collapse_to_parent = false,\n"
  "    split_lines        = true,\n"
  "    -- Caption options\n"
  "    max_capt_show_width= nil,\n"
  "    -- Display this track only if the viewport is not wider than this\n"
  "    -- number of nucleotides. Set to 0 to disable type track.\n"
  "    max_show_width     = nil,\n"
  "    -- Limit the number of tracks\n"
  "    max_num_lines      = 10,\n"
  "  },\n"
  "--------------------------------------\n"
  "  exon = {\n"
  "    -- Color definitions\n"
  "    stroke             = {red=0.0, green=0.0, blue=0.0, alpha = 1.0},\n"
  "    stroke_marked      = {red=1.0, green=0.0, blue=0.0},\n"
  "    fill               = {red=0.6, green=0.6, blue=0.9},\n"
  "    style              = \"box\",\n"
  "    -- Collapsing options\n"
  "    collapse_to_parent = true,\n"
  "    split_lines        = true,\n"
  "    -- Caption options\n"
  "    max_capt_show_width= nil,\n"
  "    -- Display this track only if the viewport is not wider than this\n"
  "    -- number of nucleotides. Set to 0 to disable type track.\n"
  "    max_show_width     = nil,\n"
  "    -- Limit the number of tracks\n"
  "    max_num_lines      = 10,\n"
  "  },\n"
  "--------------------------------------\n"
  "  CDS = {\n"
  "    -- Color definitions\n"
  "    stroke             = {red=0.0, green=0.0, blue=0.0, alpha = 1.0},\n"
  "    stroke_marked      = {red=1.0, green=0.0, blue=0.0},\n"
  "    fill               = {red=0.9, green=0.9, blue=0.2, alpha = 0.7},\n"
  "    style              = \"box\",\n"
  "    -- Collapsing options\n"
  "    collapse_to_parent = true,\n"
  "    split_lines        = true,\n"
  "    -- Caption options\n"
  "    max_capt_show_width= nil,\n"
  "    -- Display this track only if the viewport is not wider than this\n"
  "    -- number of nucleotides. Set to 0 to disable type track.\n"
  "    max_show_width     = nil,\n"
  "    -- Limit the number of tracks\n"
  "    max_num_lines      = 10,\n"
  "  },\n"
  "  intron = {\n"
  "    -- Color definitions\n"
  "    stroke             = {red=0.0, green=0.0, blue=0.0, alpha = 1.0},\n"
  "    stroke_marked      = {red=1.0, green=0.0, blue=0.0},\n"
  "    fill               = {red=1.0, green=1.0, blue=1.0},\n"
  "    style              = \"caret\",\n"
  "    -- Collapsing options\n"
  "    collapse_to_parent = true,\n"
  "    split_lines        = true,\n"
  "    -- Caption options\n"
  "    max_capt_show_width= nil,\n"
  "    -- Display this track only if the viewport is not wider than this\n"
  "    -- number of nucleotides. Set to 0 to disable type track.\n"
  "    max_show_width     = nil,\n"
  "    -- Limit the number of tracks\n"
  "    max_num_lines      = 10,\n"
  "  },\n"
  "--------------------------------------\n"
  "  repeat_region = {\n"
  "    -- Color definitions\n"
  "    stroke             = {red=0.0, green=0.0, blue=0.0, alpha = 0.0},\n"
  "    stroke_marked      = {red=1.0, green=0.0, blue=0.0, alpha = 0.0},\n"
  "    fill               = {red=0.8, green=0.4, blue=0.4, alpha = 0.0},\n"
  "    style              = \"box\",\n"
  "    -- Collapsing options\n"
  "    collapse_to_parent = false,\n"
  "    split_lines        = true,\n"
  "    -- Caption options\n"
  "    max_capt_show_width= nil,\n"
  "    -- Display this track only if the viewport is not wider than this\n"
  "    -- number of nucleotides. Set to 0 to disable type track.\n"
  "    max_show_width     = 0;\n"
  "    -- Limit the number of tracks\n"
  "    max_num_lines      = 10,\n"
  "  },\n"
  "--------------------------------------\n"
  "  LTR_retrotransposon = {\n"
  "    -- Color definitions\n"
  "    stroke             = {red=0.0, green=0.0, blue=0.0, alpha = 0.8},\n"
  "    stroke_marked      = {red=1.0, green=0.0, blue=0.0, alpha = 1.0},\n"
  "    fill               = {red=0.8, green=0.5, blue=0.5, alpha = 1.0},\n"
  "    style              = \"box\",\n"
  "    -- Collapsing options\n"
  "    collapse_to_parent = false,\n"
  "    split_lines        = true,\n"
  "    -- Caption options\n"
  "    max_capt_show_width= nil,\n"
  "    -- Display this track only if the viewport is not wider than this\n"
  "    -- number of nucleotides. Set to 0 to disable type track.\n"
  "    max_show_width     = nil,\n"
  "    -- Limit the number of tracks\n"
  "    max_num_lines      = 10,\n"
  "  },\n"
  "--------------------------------------\n"
  "  long_terminal_repeat = {\n"
  "    -- Color definitions\n"
  "    stroke             = {red=0.0, green=0.0, blue=0.0, alpha = 1.0},\n"
  "    stroke_marked      = {red=1.0, green=0.0, blue=0.0},\n"
  "    fill               = {red=0.7, green=0.7, blue=0.7},\n"
  "    style              = \"box\",\n"
  "    -- Collapsing options\n"
  "    collapse_to_parent = true,\n"
  "    split_lines        = true,\n"
  "    z_index            = 10,\n"
  "    -- Caption options\n"
  "    max_capt_show_width= nil,\n"
  "    -- Display this track only if the viewport is not wider than this\n"
  "    -- number of nucleotides. Set to 0 to disable type track.\n"
  "    max_show_width     = nil,\n"
  "    -- Limit the number of tracks\n"
  "    max_num_lines      = 10,\n"
  "  },\n"
  "  target_site_duplication = {\n"
  "    max_show_width = 0,\n"
  "  },\n"
  "--------------------------------------\n"
  "  protein_match = {\n"
  "    -- Color definitions\n"
  "    stroke             = {red=0.0, green=0.0, blue=0.0, alpha = 1.0},\n"
  "    stroke_marked      = {red=1.0, green=0.0, blue=0.0},\n"
  "    block_caption      = function(fn)\n"
  "                           return fn:get_attribute(\"name\")\n"
  "                         end,\n"
  "    fill               = function(gn)\n"
  "                           if gn:get_score() then\n"
  "                             score = gn:get_score()\n"
  "                             aval = math.max(0.1, (1-(-1*math.log(score)/100)))\n"
  "                           else\n"
  "                             aval = 1.0\n"
  "                           end\n"
  "                           type = gn:get_attribute(\"name\")\n"
  "                             if type == \"RVT_1\" or type == \"RVT_2\"\n"
  "                               or type == \"RVT_thumb\" then\n"
  "                               -- RT domain\n"
  "                               return {red=0.8, green=0.5, blue=0.5, alpha = 1.0}\n"
  "                             elseif type == \"RVP\" then\n"
  "                               -- protease domain\n"
  "                               return {red=0.5, green=0.5, blue=0.8, alpha = 1.0}\n"
  "                             elseif type == \"rve\"\n"
  "                               or type == \"Integrase_Zn\" or type == \"Integrase\" then\n"
  "                               -- integrase domain\n"
  "                               return {red=0.5, green=0.8, blue=0.8, alpha = 1.0}\n"
  "                             elseif type == \"RNase_H\" then\n"
  "                               -- RNase H domain\n"
  "                               return {red=0.8, green=0.8, blue=0.5, alpha = 1.0}\n"
  "                             else\n"
  "                               -- misc domain\n"
  "                               return {red=0.5, green=0.8, blue=0.5, alpha = 1.0}\n"
  "                            end\n"
  "                         end,\n"
  "    style              = \"box\",\n"
  "    -- Collapsing options\n"
  "    collapse_to_parent = false,\n"
  "    split_lines        = true,\n"
  "    group_by_parent    = false,\n"
  "    split_lines        = true,\n"
  "    z_index = 2,\n"
  "-- Caption options\n"
  "    max_capt_show_width= nil,\n"
  "    -- Display this track only if the viewport is not wider than this\n"
  "    -- number of nucleotides. Set to 0 to disable type track.\n"
  "    max_show_width     = nil,\n"
  "    -- Limit the number of tracks\n"
  "    max_num_lines      = 10,\n"
  "  },\n"
  "--------------------------------------\n"
  "  five_prime_splice_site = {\n"
  "    collapse_to_parent = true,\n"
  "  },\n"
  "--------------------------------------\n"
  "  primer_binding_site = {\n"
  "    -- Color definitions\n"
  "    stroke             = {red=0.9, green=0.4, blue=0.4},\n"
  "    stroke_marked      = {red=1.0, green=0.0, blue=0.0},\n"
  "    fill               = stroke,\n"
  "    style              = \"rectangle\",\n"
  "    stroke_width       = 3,\n"
  "    z_index            = 3,\n"
  "    bar_height         = 8,\n"
  "    -- Collapsing options\n"
  "    collapse_to_parent = false,\n"
  "    split_lines        = true,\n"
  "    -- Caption options\n"
  "    max_capt_show_width= nil,\n"
  "    -- Display this track only if the viewport is not wider than this\n"
  "    -- number of nucleotides. Set to 0 to disable type track.\n"
  "    max_show_width     = nil,\n"
  "    -- Limit the number of tracks\n"
  "    max_num_lines      = 10,\n"
  "  },\n"
  "--------------------------------------\n"
  "RR_tract = {\n"
  "    -- Color definitions\n"
  "    stroke             = {red=0.8, green=0.7, blue=1.0},\n"
  "    stroke_marked      = {red=1.0, green=0.0, blue=0.0},\n"
  "    fill               = stroke,\n"
  "    style              = \"rectangle\",\n"
  "    stroke_width       = 3,\n"
  "    z_index            = 3,\n"
  "    bar_height         = 8,\n"
  "    -- Collapsing options\n"
  "    collapse_to_parent = false,\n"
  "    split_lines        = true,\n"
  "    -- Caption options\n"
  "    max_capt_show_width= nil,\n"
  "    -- Display this track only if the viewport is not wider than this\n"
  "    -- number of nucleotides. Set to 0 to disable type track.\n"
  "    max_show_width     = nil,\n"
  "    -- Limit the number of tracks\n"
  "    max_num_lines      = 10,\n"
  "  },\n"
  "  --------------------------------------\n"
  "nucleotide_match = {\n"
  "    -- Color definitions\n"
  "    stroke             = {red=0.0, green=0.0, blue=0.0, alpha = 1.0},\n"
  "    stroke_marked      = {red=1.0, green=0.0, blue=0.0},\n"
  "    fill               = {red=0.9, green=0.9, blue=0.2, alpha = 0.7},\n"
  "    block_caption      = function(gn)\n"
  "                           a = gn:get_attribute(\"target\")\n"
  "                           return a\n"
  "                         end,\n"
  "    style              = \"box\",\n"
  "    stroke_width       = 1,\n"
  "    z_index            = 3,\n"
  "    bar_height         = 10,\n"
  "    -- Collapsing options\n"
  "    collapse_to_parent = false,\n"
  "    split_lines        = true,\n"
  "    -- Caption options\n"
  "    max_capt_show_width= nil,\n"
  "    -- Display this track only if the viewport is not wider than this\n"
  "    -- number of nucleotides. Set to 0 to disable type track.\n"
  "    max_show_width     = nil,\n"
  "    -- Limit the number of tracks\n"
  "    max_num_lines      = 10,\n"
  "  },\n"
  "--------------------------------------\n"
  "  reading_frame = {\n"
  "    -- Color definitions\n"
  "    stroke             = {red=0.0, green=0.0, blue=0.0, alpha = 1.0},\n"
  "    stroke_marked      = {red=1.0, green=0.0, blue=0.0},\n"
  "    fill               = {red=0.6, green=0.6, blue=0.9, alpha = 0.7},\n"
  "    block_caption      = function(gn)\n"
  "                           rng = gn:get_range()\n"
  "                           return string.format(\"%dnt (%d%s)\", rng:get_end() - rng:get_start() + 1, gn:get_attribute(\"frame\"), gn:get_strand())\n"
  "                         end,\n"
  "    style              = \"box\",\n"
  "    stroke_width       = 1,\n"
  "    z_index            = 3,\n"
  "    bar_height         = 10,\n"
  "    -- Collapsing options\n"
  "    collapse_to_parent = false,\n"
  "    split_lines        = true,\n"
  "    group_by_parent    = false,\n"
  "  },\n"
  "--------------------------------------\n"
  "  expressed_sequence_match = {\n"
  "    fill               = {red=0.5, green=0.5, blue=0.8},\n"
  "    max_show_width     = 10000,\n"
  "    max_num_lines      = 10,\n"
  "    max_capt_show_width= 5000,\n"
  "  },\n"
  "--------------------------------------\n"
  "  binding_site = {\n"
  "    fill               = {red=0.7, green=0.5, blue=0.8},\n"
  "    max_show_width     = 10000,\n"
  "    max_num_lines      = 10,\n"
  "    max_capt_show_width= 10000,\n"
  "  },\n"
  "--------------------------------------\n"
  "  SNP = {\n"
  "    fill               = {red=0.5, green=0.8, blue=0.8},\n"
  "    max_show_width     = 10000,\n"
  "    max_num_lines      = 10,\n"
  "    max_capt_show_width= 10000,\n"
  "  },\n"
  "--------------------------------------\n"
  "  deletion = {\n"
  "    fill               = {red=0.5, green=0.8, blue=0.8},\n"
  "    max_show_width     = nil,\n"
  "    max_num_lines      = 10,\n"
  "    max_capt_show_width= 10000,\n"
  "  },\n"
  "--------------------------------------\n"
  "  chromosome = {\n"
  "    fill               = {red=0.5, green=0.8, blue=0.8},\n"
  "    max_show_width     = nil,\n"
  "    max_num_lines      = 10,\n"
  "  },\n"
  "--------------------------------------\n"
  "  substitution = {\n"
  "    fill               = {red=1.0, green=0.5, blue=0.5},\n"
  "    max_show_width     = 10000,\n"
  "    max_num_lines      = 10,\n"
  "    max_capt_show_width= 1000,\n"
  "  },\n"
  "--------------------------------------\n"
  "  BED_feature = {\n"
  "    fill               = {red=0.5, green=0.5, blue=0.5},\n"
  "    style              = \"box\",\n"
  "  },\n"
  "--------------------------------------\n"
  "  BED_thick_feature = {\n"
  "    fill               = {red=0.5, green=0.5, blue=0.5},\n"
  "    style              = \"box\",\n"
  "    collapse_to_parent = true,\n"
  "  },\n"
  "--------------------------------------\n"
  "  BED_block = {\n"
  "    fill               = {red=0.5, green=1.0, blue=0.5},\n"
  "    style              = \"box\",\n"
  "    collapse_to_parent = true,\n"
  "  },\n"
  "--------------------------------------\n"
  "  GC_content = {\n"
  "    -- this is for a custom track\n"
  "    stroke             = {red=0.2, green=0.2, blue=1.0, alpha = 0.4},\n"
  "  },\n"
  "--------------------------------------\n"
  "  -- Defines various format options for drawing.\n"
  "  format =\n"
  "  {\n"
  "    -- Set the following three settings to 'false' to globally disable them.\n"
  "    split_lines = true, -- generally enable splitting\n"
  "    show_block_captions = true, -- generally show captions\n"
  "    show_track_captions = true, -- generally show track captions\n"
  "    margins = 30,      -- space left and right of diagram, in pixels\n"
  "    bar_height = 12,   -- height of a feature bar, in pixels\n"
  "    bar_vspace = 4,   -- space between feature bars, in pixels\n"
  "    track_vspace = 12, -- space between tracks, in pixels\n"
  "    ruler_font_size = 11, -- size of scale labels\n"
  "    ruler_space = 20, -- space between ruler and first track\n"
  "    block_caption_font_size = 11, -- size of block captions\n"
  "    block_caption_space = 7, -- space between blocks and their captions\n"
  "    track_caption_font_size = 12, -- size of track captions\n"
  "    track_caption_space = 15, -- space between tracks and their captions\n"
  "    scale_arrow_width = 6,     -- width of scale arrowheads, in pixels\n"
  "    scale_arrow_height = 10,   -- height of scale arrowheads, in pixels\n"
  "    arrow_width = 6,   -- width of feature arrowheads, in pixels\n"
  "    stroke_width = .5, -- width of outlines, in pixels\n"
  "    stroke_marked_width = 1.5, -- width of outlines for marked elements, in pixels\n"
  "    show_grid = true, -- shows light vertical lines for orientation\n"
  "    min_len_block = 20 , -- minimum length of a block in which single elements are shown\n"
  "    track_title_color     = {red=0.7, green=0.7, blue=0.7, alpha = 1.0},\n"
  "    default_stroke_color  = {red=0.1, green=0.1, blue=0.1, alpha = 1.0},\n"
  "    background_color      = {red=1.0, green=1.0, blue=1.0, alpha = 1.0},\n"
  "  },\n"
  "}\n";

int ltrsift_load_default_style(GtStyle *style, GtError *err)
{
  int had_err = 0;
  GtStr *str;
  gt_error_check(err);

  str = gt_str_new_cstr(default_style);
  had_err = gt_style_load_str(style, str, err);

  return had_err;
}
