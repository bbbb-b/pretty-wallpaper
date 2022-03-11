void check_shader_status(unsigned int shader);
void check_program_status(unsigned int program);
unsigned int create_shader(unsigned int shader_type, const char *src_file);
unsigned int load_texture(const char *path);
unsigned int create_compute_program(const char* fn);
unsigned int create_shader_program(const char* fn1, const char *fn2);
