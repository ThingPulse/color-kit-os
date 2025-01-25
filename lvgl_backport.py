import os
import re
import sys

# Define mappings for v8 to v7 conversions
CONVERSIONS = {
    # Function names
    r'\bcreate_screen_main\(\)': r'create_screen_main(lv_obj_t *tile)',
    r'\blv_obj_t \*parent_obj = obj;': r'lv_obj_t *parent_obj = tile;',
    r'\blv_obj_set_scrollbar_mode\(obj, LV_SCROLLBAR_MODE_OFF\);': r'//lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);',

    # Object creation
    r'\blv_obj_create\(0\)': r'lv_cont_create(tile, NULL)',
    r'\blv_label_create\((\w+)\);': r'lv_label_create(\1, NULL);\n\t\t\tlv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);',
    r'\blv_img_create\((\w+)\)': r'lv_img_create(\1, NULL)',
    r'\blv_line_create\((\w+)\)': r'lv_line_create(\1, NULL)',


    # Style and property changes
    r'\blv_obj_set_style_bg_color\((\w+),\s*([^\s,]+),\s*(LV_PART_MAIN\s*\|\s*LV_STATE_DEFAULT)\)': r'lv_obj_set_style_local_bg_color(\1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, \2)',
    r'\blv_obj_set_style_text_font\((\w+),\s*(&?[^\s,]+),\s*(LV_PART_MAIN\s*\|\s*LV_STATE_DEFAULT)\)': r'lv_obj_set_style_local_text_font(\1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, \2)',
    r'\blv_obj_set_style_text_align\((\w+),\s*([^\s,]+),\s*(LV_PART_MAIN\s*\|\s*LV_STATE_DEFAULT)\)': r'lv_label_set_align(\1, \2)',
    r'\blv_obj_clear_flag\((\w+),\s*([^\)]+)\)': r'lv_obj_set_click(\1, false)',
    r'\blv_obj_set_style_line_color\((\w+),\s*lv_color_hex\((0x[0-9a-fA-F]+)\),\s*LV_PART_MAIN\s*\|\s*LV_STATE_DEFAULT\);': r'''
    \t\tstatic lv_style_t style_\1;
    \t\tlv_style_init(&style_\1);
    \t\tlv_style_set_line_color(&style_\1, LV_STATE_DEFAULT, lv_color_hex(\2));
    \t\tlv_obj_add_style(\1, LV_LINE_PART_MAIN, &style_\1);
    ''',
    r'\blv_obj_set_style_line_width\((\w+),\s*(\d+),\s*LV_PART_MAIN\s*\|\s*LV_STATE_DEFAULT\)': r'''
    \t\tstatic lv_style_t style_\1_2;
    \t\tlv_style_init(&style_\1_2);
    \t\tlv_style_set_line_width(&style_\1_2, LV_STATE_DEFAULT, \2);
    \t\tlv_obj_add_style(\1, LV_LINE_PART_MAIN, &style_\1_2);
    ''',
    r'\blv_obj_set_style_text_color\((\w+),\s*lv_color_hex\((0x[0-9a-fA-F]+)\),\s*LV_PART_MAIN\s*\|\s*LV_STATE_DEFAULT\)': r'lv_obj_set_style_local_text_color(\1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(\2 & 0xFFFFFF))',
    r'\blv_label_set_long_mode\(obj, LV_LABEL_LONG_CLIP\)': r'lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP)',
    r'\blv_obj_set_style_border_width\(obj, 0, LV_PART_MAIN \| LV_STATE_DEFAULT\)': r'lv_obj_set_style_local_border_width(obj, 0, LV_CONT_PART_MAIN, LV_STATE_DEFAULT)',
    r'\blv_obj_set_style_bg_opa\(obj, (\w+), LV_PART_MAIN \| LV_STATE_DEFAULT\)': r'lv_obj_set_style_local_bg_opa(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, \1)',



    # Flags
    r'\bLV_OBJ_FLAG_SCROLLABLE': r'LV_CONT_FLAG_SCROLLABLE',
    r'\bLV_OBJ_FLAG_SCROLL_ELASTIC': r'LV_CONT_FLAG_SCROLL_ELASTIC',
    r'\bLV_OBJ_FLAG_SCROLL_MOMENTUM': r'LV_CONT_FLAG_SCROLL_MOMENTUM',
    r'\bLV_OBJ_FLAG_SCROLL_CHAIN_HOR': r'LV_CONT_FLAG_SCROLL_CHAIN',
    
    # Scrollbar
    #r'\blv_obj_set_scrollbar_mode\((\w+),\s*LV_SCROLLBAR_MODE_OFF\)': r'lv_obj_set_style_local_pad_right(\1, LV_CONT_PART_SCROLLBAR, LV_STATE_DEFAULT, 0)',
    
    # Labels
    r'\blv_label_set_text\((\w+),\s*([^\)]+)\)': r'lv_label_set_static_text(\1, \2)',
    r'\bLV_TEXT_ALIGN_CENTER': r'LV_LABEL_ALIGN_CENTER',
    r'\bLV_SIZE_CONTENT': r'LV_SIZE_AUTO',

    # Image
    #r'\blv_img_set_src\((\w+),\s*([^\)]+)\)': r'lv_img_set_file(\1, \2)',
    
}

# Function to comment out the content of create_screens
def comment_out_function_content(content, function_name):
    pattern = rf'void\s+{function_name}\s*\([^)]*\)\s*\{{(.*?)\}}'
    def replacer(match):
        body = match.group(1)
        commented_body = '\n'.join(f'// {line}' for line in body.splitlines())
        return f'void {function_name}() {{\n{commented_body}\n}}'

    return re.sub(pattern, replacer, content, flags=re.DOTALL)

def backport_lvgl_v8_to_v7(file_path):
    # Check if the file exists
    if not os.path.exists(file_path):
        print(f"Error: File '{file_path}' does not exist.")
        return

    # Read the content of the file
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()

    # Apply each conversion
    for pattern, replacement in CONVERSIONS.items():
        content = re.sub(pattern, replacement, content)

    # Comment out the content of the `create_screens` function
    content = comment_out_function_content(content, "create_screens")

    # Write the updated content back to the same file
    with open(file_path, 'w', encoding='utf-8') as f:
        f.write(content)

    print(f"File '{file_path}' has been successfully backported to LVGL v7.")

if __name__ == "__main__":
    # Check if a file path is provided
    if len(sys.argv) < 2:
        print("Usage: python lvgl_backport.py <file_path>")
        sys.exit(1)

    # Get the file path from the command-line arguments
    input_file = sys.argv[1]
    backport_lvgl_v8_to_v7(input_file)
