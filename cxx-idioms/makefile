TARGETS= \
		 raii \
		 print_vector \
		 file_to_string \
		 convert_char_ptr_to_double \
		 named_params \
		 copy_and_swap \
		 copy_elision \
		 map_keys_to_vector \
		 option_type \
		 if_init \
		 tuple_as_lambda \
		 print_line_of \
		 print_unpack \
		 verbose_assert \
		 range_pair \
		 try_block_function \
		 curiously_recurring_template_pattern \
		 make_templated_class \
		 non_throwing_swap \
		 named_parameters \
		 multi_stmt_macro \
		 making_new_friends \
		 sfinae \
		 has_member \
		 has_member_function \
		 execute_around_pointer \
		 static_init_fiasco \
		 expression_templates \
		 most_vexing_parse \
		 enable_if \
		 initialization_list \
		 local_class_definition \
		 vertical_or_horizontal_storage \
		 filter_types \
		 map_types \
		 named_params_2

INDEX=index.rst


CLEANFILES=$(TARGETS) $(INDEX)
CXXFLAGS=-std=c++14 -Wall -O0 -g
all:$(TARGETS) $(INDEX)

clean:
	$(RM) $(CLEANFILES) $(INDEX) *.o

index.rst:$(TARGETS:%=%.cpp)
	for f in $^ ; do printf " - $$f\n" ; done > $@
