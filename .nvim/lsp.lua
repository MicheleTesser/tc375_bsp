vim.g.__user_lsp_loaded = true

vim.api.nvim_create_autocmd({ "BufReadPost", "BufEnter" }, {
	pattern = { "*.c", "*.cpp", "*.h", "*.hpp" },
	callback = function(ev)
		local tc = os.getenv("TRICORE_LLVM")
		vim.lsp.start({
			name = "clangd-hightec",
			cmd = { tc .. "/bin/clangd" },
			root_dir = vim.fn.getcwd(),
			capabilities = vim.lsp.protocol.make_client_capabilities(),
		})
	end,
})

vim.api.nvim_create_autocmd("LspAttach", {
	callback = function(ev)
		local c = vim.lsp.get_client_by_id(ev.data.client_id)
		if c and c.name == "clangd" then
			c.stop()
		end
	end,
})
